// Definition of the Socket class

#ifndef Socket_class
#define Socket_class
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include "../lib/common.h"

class Socket
{
    public:
        Socket();
        virtual ~Socket();

        // Server initialization
        bool create();
        bool bind ( const int port );
        bool listen() const;
        bool accept ( Socket * ) const;

        // Client initialization
        bool connect ( const std::string host, const int port );

        // Data Transimission
        bool send ( const std::string ) const;
        bool sendFile ( const char * filename) const;
        bool send( const unsigned char ** d, int len) const;
        int recv ( std::string& ) const;
        int recv (void *buf , int maxlen = MAXRECV + 1) const;
        void set_non_blocking ( const bool );

        bool is_valid() const { return m_sock != -1; }

        int getFd() const;

    private:

        int m_sock;
        sockaddr_in m_addr;
        int general_recv ( void *buf, int maxlen) const;
        int setOptions();

    protected:
        char sockettype;
};
#endif
