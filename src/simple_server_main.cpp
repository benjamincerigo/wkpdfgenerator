#include "ServerSocket.h"
#include "BufferedLineReader.h"
#include "SocketException.h"
#include <iostream>
#include <cstring> 
#include <syslog.h>
int main ()
{
  syslog( 7 , "Start of the server %s", "hi");
  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

      while ( true )
	{
      syslog( 7 , "%s", "started the while");


	  ServerSocket new_sock;
	  server.accept ( &new_sock );

	  try
	    {
	      while ( true )
            {
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
            }
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
