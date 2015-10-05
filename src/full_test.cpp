#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

bool makeRequest( char * url , char * exReturn );
int main (int argc, char *argv[])
{
    int count = 0;
    try
    {

        bool suc;
        count++;
        // TEST TO SEE IF  there is a bad request returned when the uid and the cid are not the same in the url
        suc = makeRequest( "https://secure.equidam.dev/report/index.php?uid=101&cid=100&t=ahrgf4betqevm3zzkpj5ynuxbs7krkkr&pdf=true\r\n", BAD_REQUEST);
        if( !suc ){
            throw SocketException( 500, "");
        }
        count++;
        // TEST TO SEE IF  there is a bad request when the url is not correct
        suc = makeRequest( "https://secre.equidam.dev/report/index.php?uid=100&cid=100&t=ahrgf4betqevm3zzkpj5ynuxbs7krkkr&pdf=true\r\n", BAD_REQUEST);
        if( !suc ){
            throw SocketException( 500, "");
        }
        count++;
        // Test of the kill the printer
        suc = makeRequest( KILLPRINTERTEST, BAD_REQUEST);
        if( !suc ){
            throw SocketException( 500, "");
        }
        count++;
        suc = makeRequest( "https://secure.equidam.dev/report/index.php?uid=100&cid=100&t=ahrgf4betqevm3zzkpj5ynuxbs7krkkr&pdf=true\r\n", "pdf");
        if( !suc ){
            throw SocketException( 500, "");
        }
        log_info("Report made for the uid 100, cid 100 must be checked");
        count++;
        suc = makeRequest( "https://secure.equidam.dev/report/index.php?aid=2&uid=2&cid=6&t=ahrgf4betqevm3zzkpj5ynuxbs7krkkr&pdf=true\r\n", "pdf");
        if( !suc ){
            throw SocketException( 500, "");
        }
        log_info("Report made for the uid 2, cid 6, aid 2 must be checked");
    } catch ( SocketException& e )
    {

       char ret[1000];
       size_t s = 1000;
       e.returnMessage( ret , s );
       err_sys( "error on count: %d Message: %s", count , ret); 
    }
    return 0;
}

bool makeRequest( char * url , char * exReturn ){
    ClientSocket client_socket ( "localhost", 30000 );
    std::string reply = "ENDOFFILE";
    client_socket << url;
    client_socket << "ENDOFFILE\r\n";
    while( reply.length() > 0 ){
        client_socket >> reply;
    }
    if( strcmp( "pdf", exReturn) == 0 ){
        return true;
    }
    fprintf( stdout, "expect: %s", exReturn );
    fprintf( stdout, "reply: %s", reply.c_str() );
    if( strcmp( exReturn, reply.c_str() ) != 0){
        char c[300];
        snprintf( c, 300, "%s %d returned: %s", "not corrcect" , errno, reply.c_str());
        throw SocketException ( 500 , c);
        return false;
    }
    return true;
}
