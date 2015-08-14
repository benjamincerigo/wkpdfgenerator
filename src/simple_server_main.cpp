#include "ServerSocket.h"
#include "BufferedLineReader.h"
#include "SocketException.h"
#include <stdio.h>
#include <cstring> 
#include "../lib/common.h"

void sig_chld(int signo);
int main ()
{
    pid_t pid;
  syslog( 7 , "Start of the server %s", "hi");
  try
    {
      // Create the socket
      ServerSocket server ( 30000 );
      Signal(SIGCHLD, sig_chld); // kill the zombie forks

        while ( true )
        {
          syslog( 7 , "%s", "started the while");

          ServerSocket new_sock;
        if( !(server.accept ( &new_sock )) ){ // Blocking so will stop with
            if( errno == EINTR )
                // This is a signal interupt
                continue; // Return to the for
            else
            {
                // must log the execption
             }
        }

          if( (pid = fork()) == 0) { /* Create the fork to prcess the request*/
            /* CHILD ONLY */
            try
              {
                server.~ServerSocket(); // close the listening socket for the child
                syslog( 7 , "%s", "started the while2");
                std::string data;
                char url[ 2000 ];
                int n, cnt = 0;
                BufferedLineReader blr = BufferedLineReader( new_sock );
                while((n = blr.readLine( url , 2000 )) > 0 )
                {
                    std::string str = url;
                    syslog( 7 , "Line found num: %d", n);
                    syslog( 7 , "Line found str: %s", url);
                    cnt++;
                    if( strcmp( url , "ENDOFFILE" ) ){
                        
                        syslog( 7 , "ENDOFFILEFOUND");
                        break;
                    }
                }
                syslog( 7 , "Line found finished");
                data = "EOF";
                syslog( 7 , "the returned:%s", data.c_str());
                new_sock << data;
                  exit(0);
              }
            catch ( SocketException& e ) {
                char ret[300];
                e.returnMessage( ret , (size_t)300);
                printf("%s", ret);
                syslog( 7 , "Exception Caught %s", ret);
                exit(0);
            }
            catch ( SocketTimeOut& e ) {
                syslog( 7 , "Exception Caught %s", e.message());
                new_sock.~ServerSocket();
                exit(0);
            }
            /* Child Socket is closed and Fork is exited */
          }
          new_sock.~ServerSocket(); // Close the socket in Parent so that waiting for an new request
      }
    }
    catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
  return 0;
}

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
