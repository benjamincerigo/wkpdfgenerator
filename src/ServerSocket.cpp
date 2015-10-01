// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include <syslog.h>
#include <errno.h>
#include <stdio.h>

ServerSocket::ServerSocket()  
{

    Socket::sockettype = 's';
}

ServerSocket::ServerSocket ( int port )  
{
    Socket::sockettype = 's';
    if ( ! Socket::create() )
    {
        throw SocketException ( 500, "Could not create server socket." );
    }

    if ( ! Socket::bind ( port ) )
    {
        throw SocketException ( 500 , "Could not bind to port." );
    }

    if ( ! Socket::listen() )
    {
        throw SocketException ( 500 , "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
    if ( ! Socket::send ( s ) )
    {
        char c[300];
        snprintf( c, 300, "%s %d", "Could not write to socket." , errno);
        throw SocketException ( 500 , c);
    }
    return *this;

}

const ServerSocket& ServerSocket::send ( const unsigned char ** d, int len) const
{
    if ( ! Socket::send ( d, len  ) )
    {
        char c[300];
        snprintf( c, 300, "%s %d", "Could not write to socket." , errno);
        throw SocketException ( 500 , c);
    }
    return *this;

}

bool ServerSocket::sendFile( const char * filename  ) const
{ 
    return  Socket::sendFile( filename);
}

const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
    if ( ! Socket::recv ( s ) )
    {
        throw SocketException ( 500 , "Could not read from socket." );
    }
    return *this;
}

bool ServerSocket::accept ( ServerSocket *sock )
{
    return Socket::accept ( sock ); 
}
