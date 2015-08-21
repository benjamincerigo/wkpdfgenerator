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
int printpdf(char * url, const unsigned char ** d ) {
	int status = 0;
	log_info("Pdf print started with url: %s", url);
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
		char outstr[16];
		memset( &outstr , 0 , sizeof(outstr));
		snprintf( outstr, 15, "test%d.pdf", getpid());
		wkhtmltopdf_set_global_setting(gs, "out", outstr);
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
	wkhtmltopdf_set_object_setting(os, "load.jsdelay", "10000");
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
		status = -1;
	} else {
		log_info("doing else");
		status = wkhtmltopdf_get_output(c, d);
		err_sys("status = %d", status);
	}

	/* Destroy the converter object since we are done with it */
	wkhtmltopdf_destroy_converter(c);

	/* We will no longer be needing wkhtmltopdf funcionality */
	wkhtmltopdf_deinit();

	return status;
}
