/*
 * BufferedLineReader 
 * Implementaiton
 *
 */

#include "BufferedLineReader.h"
#include <syslog.h>
#include "SocketException.h"

BufferedLineReader::BufferedLineReader ( const Socket &s): unread_cnt(0) 
{
    socket = s;
}

BufferedLineReader::~BufferedLineReader ()
{

}

// Looks for the \r\n forthe nd of the line
bool BufferedLineReader::endOfLine( const char * ptr )
{

    if( *ptr == '\n' && returnfound == true )
    {
        returnfound = false;
        return true; // Mean that we found the \r\n
    }
    else if( *ptr =='\r' ){
        returnfound = true;
        return false;
    } else {
        returnfound = false;
        return false;
    }
}

int BufferedLineReader::readLine ( char * vptr, int maxlen)
{
    ssize_t n, rc;
    char c;
    char *ptr;
    // Reset the last
    returnfound = false; // inportant as will carry from the last
    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = internal_read(&c)) == 1) {
            *ptr++ = c;
            if( endOfLine( &c ) ) 
                break;
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1);  /* EOF, n - 1 bytes were read */
        } else
            return(-1);     /* error, errno set by read() */
    }

    *ptr = 0;   /* null terminate like fgets() */
    if( removenewline == true )
    {
        /* Remove the new line charateres form the end */
        ptr--;
        *ptr-- = 0;
        *ptr = 0;
    }
    if( n == maxlen )
        return -1;
    if ( n < 0)
    {
        // The line is more then the given buffer size
        throw SocketException( 413, "Entity Too Large");
    }
    return (n);
}

int BufferedLineReader::internal_read( char *ptr )
{
    if (unread_cnt <= 0) {
        
        if ( (unread_cnt = socket.recv( read_buf )) < 0) 
        {
            if( unread_cnt == -1 )
            {
                return(-1);
            } else if (unread_cnt == 0)
                return(0);
        }
        read_ptr = read_buf;
    }
    unread_cnt--;
    *ptr = *read_ptr++;
    return(1);
}
// Debuging function
void BufferedLineReader::helplog( int pos )
{
    syslog( 7 , "pos %d", pos);
    syslog( 7 , "return found %d", returnfound);
    syslog( 7 , "unread_cnt %d", unread_cnt);
    syslog( 7 , "read_ptr %s", read_ptr);
}
