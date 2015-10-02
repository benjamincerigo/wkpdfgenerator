/*
 * Url Interface
 */
#include "../lib/common.h"

bool checkUrl( char * url , int len , char  * query, int qlen);
bool startsWith( const char * url , const char * com, int len);
bool findQueryPartDigit( const char * url , const char * part, char * found);
bool preformregex( const char * tomatch, const char * regex , const int reglen, char * found, const int foundlen, int TYPE);
void eqconcat(char *s1, char *s2);
bool getQueryParams( char * url  , char * query, int qlen);
