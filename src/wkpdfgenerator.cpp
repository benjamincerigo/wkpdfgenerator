#include "ServerSocket.h"
#include "BufferedLineReader.h"
#include <stdio.h>
#include <cstring> 
#include <sys/types.h>
#include <unistd.h>
#include "Url.h" 
#include "../lib/common.h"
#include "pdf.h"
#include <sys/stat.h>

const int bufsize = 2000;
const int givenport = 30000;

void sig_chld(int signo);

#define LOCALPATH "/vagrant/reports"
int main ( int argc, char **argv)
{

    pid_t pid;
    daemon_init(argv[0], LOG_LOCAL1);
    // Logg starting of the server
    log_info("PDF wkhtmltopdf started");
    if( chdir(LOCALPATH) != 0 ){
        if( mkdir( LOCALPATH , S_IWUSR| S_IXUSR | S_IRUSR | S_IROTH) != 0){
            err_sys( "cannot find or create the directory for reports" );
            exit(-1);
        }
    }
    try
    {
        // Create the socket
        ServerSocket server ( givenport );
        Signal(SIGCHLD, sig_chld); // kill the zombie children that are left after they exit
        while ( true )
        {
            /* New socket created for each connection */
            ServerSocket new_sock;
            if( !(server.accept ( &new_sock )) ){ // Blocking so will stop while and wait for an request
                if( errno == EINTR )
                    // This is a signal interupt
                    continue; // Return to the while so wait for the accept
                else
                {
                    // must log the on the accept
                    err_sys("Error in the accept of the Server"); 
                    continue;
                }
            }

            if( (pid = fork()) == 0) { /* Create the fork to process the request*/
                /* CHILD ONLY */
                try
                {
                    log_info("New child process started: %d", getpid());
                    server.~ServerSocket(); // close the listening socket for the child because it does not need it

                    std::string data; // Return data
                    char linecheck[ bufsize ]; // line buffer
                    char url[ bufsize ]; // url
                    int n, cnt = 0; /* to help with the readline */
                    int sizeofdata = 300; /* size of the pdfdata */
                    int qlen = 200; /* max size of query */
                    char queryparams[qlen]; /* the query parameter to be used for the name of file */
                    char pdfdata[sizeofdata]; /* the name of the file that will be passed arount */
                    // clean the memory
                    memset( queryparams, '\0', sizeof(char)*qlen );
                    memset( pdfdata, '\0', sizeof(char)*sizeofdata );
                    memset( url, '\0', sizeof(char)*bufsize );
                    memset( linecheck, '\0', sizeof(char)*bufsize );
                    data = BAD_REQUEST;
                    /* Read each line 
                     * Lines are read until ENDOFFILE is found
                     * Can also exit on exception of no end of file or timeout
                     *
                     * LAST line found will be url
                     * 
                     */
                    BufferedLineReader blr = BufferedLineReader( new_sock );
                    while((n = blr.readLine( linecheck , bufsize )) > 0 )
                    {
                        cnt++;
                        // Looking for ENDOFFILE
                        if( startsWith( linecheck , "ENDOFFILE" , bufsize) ){
                            log_info("EOF recieved for pid: %d", getpid());
                            break;
                        } else {
                            // Copy the line into url
                            strncpy( url, linecheck , bufsize);
                        }
                    }
                    // Check the url to see if it asecaable
                    log_info("Url found for pid: %d , url %s", getpid(), url);
                    if( checkUrl( url , bufsize , queryparams, qlen) == true ){
                        int len;
                        data = SERVER_ERR;
                        len = printpdf( url, pdfdata , sizeofdata, queryparams);
                        if( strlen( pdfdata ) <= 0){
                            err_sys("ERROR PDF FOR FAILD INTERNAL IN PRINTER for url: %s: pdfdata: %d", url, strlen(pdfdata));
                            data = SERVER_ERR;
                        } else if( strcmp( BAD_REQUEST , pdfdata) == 0){
                            err_sys("ERROR PDF FOR FAILD BAD REQUEST FROM PRINTER for url: %s: pdfdata: %d", url, strlen(pdfdata));
                            data = BAD_REQUEST;
                        } else {
                            // THe file name is good so send the file
                           if( new_sock.sendFile( pdfdata )){
                            exit(-1); 
                           } else {
                               data = SERVER_ERR;
                           }
                        }
                    } else {
                        // save and return the error
                        log_notice("NOTICE Url not authorsied pid: %d , url %s", getpid(), url);
                        data = BAD_REQUEST;
                    }
                    new_sock << data;
                    log_info("Fail for child process: %d served message: %s", getpid(), data.c_str());
                    exit(0); // exit the child once the return is sent back
                } catch ( SocketException &e ) {
                    // Socketexception of the child should be loged
                    char ret[300];
                    e.returnMessage( ret , (size_t)300);
                    err_sys( "Socekt Error with messages: %s for pid: %d", ret, getpid() );
                    exit(0);
                }
                catch ( SocketTimeOut& e ) {
                    // Socket time out Needs to be logged
                    err_sys( "408 Request Timeout for pid: %d", getpid()  );
                    exit(0);
                }
                /* Child Socket is closed and Fork is exited */
            }
            new_sock.~ServerSocket(); // Close the socket in Parent so that waiting for an new request
        }
    }
    catch ( SocketException& e )
    {
        // Log the Exceptions of the Server This is a bad excetion that should email us as the server is down
        char ret[1000];
        e.returnMessage( ret , (size_t)1000);
        err_sys( "Socket Error with messages: %s for pid: %d", ret, getpid() );
    }
    return 0;
}

/*
 * This Funciton is a callback that will kill the zombie when the child process has ended
 */
void
sig_chld(int signo)
{
    pid_t	pid;
    int		stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        log_info("Child Process terminated: %d", pid);

    return;
}
