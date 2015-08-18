/*
 * Buffered Line Reader is a basic line reader form an input stream
 *
 */
#include "Socket.h"

const bool removenewline = true;

class BufferedLineReader
{
    private:
        Socket socket;
        bool returnfound; // Is a bool so that we can find the \r\n
        int unread_cnt; // Will count the unread char in the current buffer
        char * read_ptr;  // This is the pointer that will transfer from the readbuf to the given array
        char read_buf[ MAXRECV + 1 ]; // Read buf to holder the current values
        int internal_read( char *ptr);
        void helplog( int pos);
        bool endOfLine( const char *ptr);
    public:
      BufferedLineReader(const Socket &s);
      virtual ~BufferedLineReader();
      int readLine( char * vptr, int maxlength );
};

