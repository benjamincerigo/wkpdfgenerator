// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include <syslog.h>

ServerSocket::ServerSocket()  
{

  Socket::sockettype = 's';
}

ServerSocket::ServerSocket ( int port )  
{
  Socket::sockettype = 's';
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
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
      throw SocketException ( "Could not write to socket." );
    }
  syslog( 7 , "ServerSocket sent %s", s.c_str());

  return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }


  return *this;
}

void ServerSocket::accept ( ServerSocket *sock )
{

  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}
