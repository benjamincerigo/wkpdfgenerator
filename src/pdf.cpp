/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 *
 * Copyright 2010, 2011 wkhtmltopdf authors
 *
 * This file is part of wkhtmltopdf.
 *
 * wkhtmltopdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * wkhtmltopdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with wkhtmltopdf.  If not, see <http: *www.gnu.org/licenses/>.
 */

/* This is a simple example program showing how to use the wkhtmltopdf c bindings */
#include <stdbool.h>
#include <stdio.h>
#include <wkhtmltox/pdf.h>
#include "../lib/common.h"
const bool fileout = false;


/* Print a message to stderr when an error occures */
void error(wkhtmltopdf_converter * c, const char * msg) {
	err_sys("Error: %s for pid: %d", msg, getpid());
}

/* Print a message to stderr when a warning is issued */
void warning(wkhtmltopdf_converter * c, const char * msg) {
	log_notice("Warning: %s for pid: %d", msg, getpid());
}
/* Print a message to stderr when a warning is issued */
void progress_changed(wkhtmltopdf_converter * c, int p) {
}
void phase_changed(wkhtmltopdf_converter * c) {
	int phase = wkhtmltopdf_current_phase(c);
	log_info("Phase chaged %s for pid: %d", wkhtmltopdf_phase_description(c, phase), getpid());
}

/* Print the pdf*/
int printpdf(char * url, char * d , const int length) {
	int fd[2];
	pipe(fd);
	pid_t pID = fork();
	if (pID == 0)                // child
	{
		bool fileout = true;
		int sizeofname = 20;
		char outstr[20];
		/* Child process closes up input side of pipe */
		close(fd[0]);
		// THis is the vfork that will do the actuall convert of the pdf beacuse if it is kill un exceptatly we can still return
		log_info("Pdf print started with url: %s pid: %d", url, getpid());
		wkhtmltopdf_global_settings * gs;
		wkhtmltopdf_object_settings * os;
		wkhtmltopdf_converter * c;

		/* Init wkhtmltopdf in graphics less mode */
		wkhtmltopdf_init(false);

		/*
		 * Create a global settings object used to store options that are not
		 * related to input objects, note that control of this object is parsed to
		 * the converter later, which is then responsible for freeing it
		 */
		gs = wkhtmltopdf_create_global_settings();
		/* We want the result to be storred in the file called test.pdf */
		wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
		if( fileout )
		{
			char outstrreal[sizeofname];
			memset( &outstr , 0 , sizeof(outstr));
			snprintf( outstr, sizeofname, "test%d.pdf", getpid());
			strncpy(outstrreal, outstr, sizeofname);
			log_info( "filenamebefore %s", outstrreal );
			log_info( "filenamebefore real %s", outstr );
			wkhtmltopdf_set_global_setting(gs, "out", outstrreal);
			log_info( "filenameafter %s", outstr );
			log_info( "filenameafter real %s", outstrreal );
		}
		wkhtmltopdf_set_global_setting(gs, "web.enableJavascript", "true");
		wkhtmltopdf_set_global_setting(gs, "margin.top", "0cm");
		wkhtmltopdf_set_global_setting(gs, "margin.bottom", "0cm");
		wkhtmltopdf_set_global_setting(gs, "margin.left", "0cm");
		wkhtmltopdf_set_global_setting(gs, "margin.right", "0cm");
		/*
		 * Create a input object settings object that is used to store settings
		 * related to a input object, note again that control of this object is parsed to
		 * the converter later, which is then responsible for freeing it
		 */
		os = wkhtmltopdf_create_object_settings();
		/* We want to convert to convert the qstring documentation page */
		wkhtmltopdf_set_object_setting(os, "page", url);
		wkhtmltopdf_set_object_setting(os, "load.jsdelay", "30000");
		wkhtmltopdf_set_object_setting(os, "load.stopSlowScript", "false");

		/* Create the actual converter object used to convert the pages */
		c = wkhtmltopdf_create_converter(gs);

		/* Call the error function when an error occures */
		wkhtmltopdf_set_error_callback(c, error);

		/* Call the warning function when a warning is issued */
		wkhtmltopdf_set_warning_callback(c, warning);
		/* Call the progress_changed function when progress changes */
		wkhtmltopdf_set_progress_changed_callback(c, progress_changed);

		/* Call the phase _changed function when the phase changes */
		wkhtmltopdf_set_phase_changed_callback(c, phase_changed);

		/*
		 * Add the the settings object describing the qstring documentation page
		 * to the list of pages to convert. Objects are converted in the order in which
		 * they are added
		 */
		wkhtmltopdf_add_object(c, os, NULL);

		/* Perform the actual convertion */
		if (!wkhtmltopdf_convert(c))
		{
			err_sys("ERROR PDF Converstionafail for url: %s", url);
			/* Output possible http error code encountered */
			err_sys("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));
			//len = -1;
		} else {
			log_info("SUCCESSFULL MADE REPORT for url: %s", url);
			if( !fileout ){
				//len = wkhtmltopdf_get_output(c, d);
			}
		}

		/* Destroy the converter object since we are done with it */
		wkhtmltopdf_destroy_converter(c);

		/* We will no longer be needing wkhtmltopdf funcionality */
		wkhtmltopdf_deinit();
		/* Send "string" through the output side of pipe */
		write(fd[1], outstr, sizeofname);
		exit(1);
	} else if( pID < 0 )
	{
		err_sys("ERROR PDF FOR FAILD");
		//status = -1; // Bcause there is an error to return
	} else {
		int status;
		/* Parent process closes up output side of pipe */
		close(fd[1]);

		log_info( "doing the wait" );
		waitpid( pID,  &status , WNOHANG | WUNTRACED |WCONTINUED );
		if (WIFEXITED(status)) {
			log_info("exited, status=%d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			log_info("killed by signal %d\n", WTERMSIG(status));
		} else if (WIFSTOPPED(status)) {
			log_info("stopped by signal %d\n", WSTOPSIG(status));
		} else if (WIFCONTINUED(status)) {
			log_info("continued\n");
		}
		/* Read in a string from the pipe */

		read(fd[0], d, length);
		log_info("tmp file in parent: %s", d );
		
	}
	return 1;
}
