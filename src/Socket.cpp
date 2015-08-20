// Implementation of the Socket class.

#include "Socket.h"
#include "../lib/common.h"

Socket::Socket() :
  m_sock ( -1 )
{
  memset ( &m_addr,
       0,
       sizeof ( m_addr ) );
}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

int Socket::setOptions()
{
    // Set the time out for the recv and send
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int st = setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    st  = setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    return st;
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,
            SOCK_STREAM,
            0 );
  if ( ! is_valid() )
    return false;
  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;


  return true;

}



bool Socket::bind ( const int port )
{
  if ( ! is_valid() )
    {
      return false;
    }
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
                 ( struct sockaddr * ) &m_addr,
                 sizeof ( m_addr ) );
  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}

bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }
  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );
  if ( listen_return == -1 )
    {
      return false;
    }
  return true;
}

bool Socket::accept ( Socket *new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket->m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
  if ( new_socket->m_sock <= 0 )
  {
      return false;
  }
  else
  {
    new_socket->setOptions();
    return true;
  }
}


bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            throw SocketTimeOut();
        }
      return false;
    }
  else
    {
      return true;
    }
}
bool Socket::send ( const unsigned char ** d, int len) const
{
    log_info("sending the buffer: %p, len: %d pid: %d", d, len, getpid());
  int status = ::send ( m_sock, *d,  len , MSG_NOSIGNAL );
  if ( status == -1 )
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            throw SocketTimeOut();
        }
      return false;
    }
  else
    {
      return true;
    }
}

int Socket::general_recv(void *buf, int maxlen) const
{
    memset ( buf, 0,  maxlen );
    int status = ::recv ( m_sock, buf, MAXRECV, 0 );
  if ( status == -1 )
  {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            send( "408 Timeout");
            throw SocketTimeOut();
        }
      errno = 0;
      return -1;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      return status;
    }
}

int Socket::recv ( std::string& s ) const
{
    char buf [ MAXRECV + 1 ];
    s = "";
    memset ( buf, 0, MAXRECV + 1 );
    int status = general_recv ( buf , MAXRECV +1);
    if ( status > 0)
        s = buf;
    return status;
}

int Socket::recv( void *buf , int maxlen ) const
{
    return general_recv( buf , maxlen );
}

bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::set_non_blocking ( const bool b )
{
  int opts;
  opts = fcntl ( m_sock,
         F_GETFL );
  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
      F_SETFL,opts );

}

int Socket::getFd() const
{
    return m_sock;
}
