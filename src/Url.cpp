/* Url check funcitonallity */ 
#include "../lib/common.h"
#include "Url.h"

#include <regex.h>        
#include <string.h>

#define GETMATCH 0
#define GETMATCHTOEND 1


/*
 * Function check is the url is legit and gets the query params
 */
bool checkUrl( char * url, int len , char * query, int qlen)
{
    const int numofchecks = 5;
    bool found;
    // Legitimate urls
    const char check[numofchecks][30] = {
        "https://www.equidam.com",
        "https://secure.equidam.com", 
        "https://badger.equidam.com", 
        "https://secure.equidam.dev", 
        "https://erasmus.equidam.com"
    };
    // Special test case
    if( strcmp( KILLPRINTERTEST, url ) == 0){
        return true;
    }
    // Get the query params for the name of the pdf file
    found = getQueryParams( url, query, qlen);
    log_info("Found Query is: %s", query );
    if( found == false ){
        return false;
    }
    for( int i = 0; i < numofchecks; i++ ){
        if( (startsWith( url , check[i] , len)) == true) 
            return true;
    }
    return false;
}

bool startsWith( const char * url , const char * com , int len)
{
    return strncmp(com, url, strlen(com)) == 0;
}

// Get the query params for the 
// uid, cid, and aid
// only return false (give error ) if the uid or cid is not correct/given
bool getQueryParams( char * url  , char * query, int qlen){
    bool found;
    char queryarray[3][100];
    int addcount = 3;
    found = findQueryPartDigit( url, "uid", queryarray[0]);
    if( !found ){
        return false;
    }
    found = findQueryPartDigit( url, "cid", queryarray[1]);
    if( !found ){
        return false;
    }
    found = findQueryPartDigit( url, "aid", queryarray[2]);
    if( !found ){
        addcount = 2;
    }
    int i;
    int alen = 0;
    char suffix[] = "-";
    for( i = 0; i < addcount; i++){
        alen = alen + strlen( queryarray[i] );
        if( alen > qlen ){
            //fprintf( stdout, "query string is not big enough for queries alen: %d qlen %d ", alen, qlen);
            return false;
        }
        eqconcat( query, queryarray[i] );
        eqconcat( query, suffix);
    }
    //fprintf( stdout, "query string: %s\n", query );
    return true;
}


/* Function to find the query with regex of the degits*/
bool findQueryPartDigit( const char * url, const char * part , char * found){
    int reti;
    char regstring[ 20 ]; 

    /* Make the regex */
    snprintf( regstring, 20 , "%s=[0-9]*", part);
    /* Execute regular expression  for the <id type>=number*/
    char found1[ 100 ];
    reti = preformregex(url, regstring, 20, found1, 100,  GETMATCH );
    if (!reti) {
        return false;
    }
    //fprintf(stdout, "match found: %s\n", found1);
    char regstring2[20];
    snprintf( regstring2, 20 , "%s=", part);
    reti = preformregex(found1, regstring2 , 4, found, 100, GETMATCHTOEND);
    if (!reti) {
        return false;
    }
    return true;
}

// Preform the regex to get the params
bool preformregex( const char * tomatch, const char * regstring , const int reglen, char * found, const int foundlen, int TYPE)
{
    int reti;
    char msgbuf[100];
    const char * match = tomatch; 
    size_t c = 2;
    regex_t regex;
    regmatch_t matchs[c];
    /* Compile regular expression */
    reti = regcomp(&regex, regstring, reglen);
    if (reti) {
        err_sys("Could not compile regex\n");
        return false;
    }

    /* Execute regular expression */
    reti = regexec(&regex, tomatch , c, matchs, REG_EXTENDED);
    if (!reti) {
        if( TYPE == 0 ){
            int start;
            int finish;
            int diff;
            start = matchs[0].rm_so + (match - tomatch);
            finish = matchs[0].rm_eo + (match - tomatch);
            diff = finish - start;
            if( diff > foundlen ){
                err_sys("Could not Copy given\n");
                return false;
            }
            //fprintf(stdout, "start: %d finish: %d diff: %d\n", start, finish, diff);
            memcpy( found, &match[ start ] , finish ); // copy the match
            found[ diff ] = '\0'; // add the null value to the end of the string
            //fprintf(stdout, "match inside: %s\n", found);
        } else {
            int start;
            int finish;
            int diff;
            start = matchs[0].rm_eo + (match - tomatch);
            finish = strlen(tomatch);
            diff = finish - start;
            if( diff > foundlen ){
                err_sys("Could not Copy given\n");
                return false;
            }
            //fprintf(stdout, "start: %d finish: %d diff: %d\n", start, finish, diff);
            memcpy( found, &match[ start ] , finish ); // copy the match
            found[ diff ] = '\0'; // add the null value to the end of the string
            //fprintf(stdout, "match inside: %s\n", found);
        }
        return true;
    }else if (reti == REG_NOMATCH) {
        return false;
    }else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        err_sys("Regex match failed: %s\n", msgbuf);
        return false;
    }
}
void eqconcat(char *s1, char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    if( len1 == 1 ){
        memcpy(s1, s2, len2+1);//+1 to copy the null-terminator
    } else {
        memcpy(s1+len1, s2, len2+1);//+1 to copy the null-terminator
    }
}
