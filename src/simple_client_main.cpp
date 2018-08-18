#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

int main (int argc, char *argv[])
{
    try
    {

        ClientSocket client_socket ( "localhost", 30000 );

        std::string reply = "EOF";
        try
        {
            client_socket << "https://www.google.nl\r\n";
            client_socket << "ENDOFFILE\r\n";
            while( reply.length() > 0 ){
                client_socket >> reply;
                std::cout << reply;
            }
        }catch ( SocketException& e ) {

        }
    } catch ( SocketException& e )
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
    return 0;
}
