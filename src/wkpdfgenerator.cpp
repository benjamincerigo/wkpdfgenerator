#include "ServerSocket.h"
#include "BufferedLineReader.h"
#include <stdio.h>
#include <cstring> 
#include "Url.h" 
#include "../lib/common.h"
#include "pdf.h"

const int bufsize = 2000;
const int givenport = 30000;

void sig_chld(int signo);

int main ( int argc, char **argv)
{

    pid_t pid;
    daemon_init(argv[0], LOG_LOCAL1);
    // Logg starting of the server
    log_info("PDF wkhtmltopdf started");
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
                    log_info("New child prcess started: %d", getpid());
                    server.~ServerSocket(); // close the listening socket for the child because it does not need it

                    std::string data; // Return data
                    char linecheck[ bufsize ]; // line buffer
                    char url[ bufsize ]; // url
                    int n, cnt = 0;
                    const unsigned char * pdfdata;
                    
                    
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
                        if( startsWith( linecheck , "ENDOFFILE" , 2000) ){
                            log_info("EOF recieved for pid: %d", getpid());
                            break;
                        } else {
                            // Copy the line into url
                            strncpy( url, linecheck , 2000);
                        }
                    }
                    // Check the url to see if it asecaable
                    log_info("Url found for pid: %d , url %s", getpid(), url);
                    if( checkUrl( url , 2000 ) == true ){
                        int len;
                        data = "200 Success";
                        len = printpdf( url, &pdfdata );
                        log_info("len = %d", len);
                        if( len <= 0)
                        {
                            data = "500 Internal Server Error";
                        } else {
                            log_info("sending the buffer");
                             new_sock.send( &pdfdata, len );
                        }
                    } else {
                        // save and return the error
                        log_notice("NOTICE Url not authorsied pid: %d , url %s", getpid(), url);
                        data = "401 Unauthorized";
                    }
                    new_sock << data;
                    log_info("Success for child process: %d", getpid());
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
