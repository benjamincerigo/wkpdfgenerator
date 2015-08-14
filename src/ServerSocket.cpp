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
      throw SocketException ( 1, "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( 1 , "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( 1 , "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  syslog( 7 , "ServerSocket sending %s", s.c_str());
  if ( ! Socket::send ( s ) )
    {
        char c[300];
    snprintf( c, 300, "%s %d", "Could not write to socket." , errno);
        
      throw SocketException ( 1 , c);
    }
  syslog( 7 , "ServerSocket sent %s", s.c_str());

  return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( 1 , "Could not read from socket." );
    }


  return *this;
}

void ServerSocket::accept ( ServerSocket *sock )
{

  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( 1 , "Could not accept socket." );
    }
}
