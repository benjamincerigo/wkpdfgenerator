/* Url check funcitonallity */ 
#include "../lib/common.h"

bool startsWith( const char * url , const char * com, int len);

const int numofchecks = 5;
 
bool checkUrl( char * url, int len )
{
    const char check[numofchecks][30] = {
        "https://www.equidam.com",
        "https://secure.equidam.com", 
        "https://badger.equidam.com", 
        "https://secure.equidam.dev", 
        "https://erasmus.equidam.com"
    };
    for( int i = 0; i < numofchecks; i++ ){
        if( (startsWith( url , check[i] , len)) == true) 
            return true;
    }
    return false;
}

bool startsWith( const char * url , const char * com , int len)
{
    int i;
    i = strncmp(com, url, strlen(com));
    return strncmp(com, url, strlen(com)) == 0;
}



