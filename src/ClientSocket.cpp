// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>


ClientSocket::ClientSocket ( std::string host, int port )
{
    Socket::sockettype = 'c';
  if ( ! Socket::create() )
    {
      throw SocketException ( 1, "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( 2, "Could not bind to port." );
    }

}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( 3, "Could not write to socket." );
    }

  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( 4,  "Could not read from socket." );
    }

  return *this;
}
