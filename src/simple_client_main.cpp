#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

int main ()
{
    time_t starttime = time(NULL);
    try
    {

        ClientSocket client_socket ( "localhost", 30000 );

        std::string reply;

        try
        {
          client_socket << "https://secure.equidam.dev/report/index.php\r\n";
          client_socket << "ENDOFFILE\r\n";


          client_socket >> reply;
        }catch ( SocketException& e ) {

          std::cout << "Exception was caught:" << e.description() << "\n";
        
        }
          std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
    } catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }
    time_t endtime = time(NULL);
    time_t diff;
    diff = endtime - starttime;
    printf("%lld\n", (long long) diff );
    return 0;
}
