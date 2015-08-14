// SocketException class


#include "../lib/common.h"
#include "SocketException.h"

int SocketException::returnMessage( char * ret ,size_t limit)
{
    snprintf(ret, limit, "%d %s", m_c,  m_s.c_str());
    syslog( 7 , "Exception return message %s", ret);
    return sizeof(ret);
}
