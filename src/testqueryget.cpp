#include "../lib/common.h"
#include "Url.h" 

bool queryParamsTest( char * url , char * cmp , bool expectedfound);
int main ( int argc, char **argv)
{
    int count = 1;
    if( !queryParamsTest( "https://secure.equidam.dev/report/index.php?aid=103&uid=101&cid=102&t=ahrgF4BEtQevm3ZzkpJ5YNUxBS7kRKkr&pdf=true", "101-102-103-", true )){
        fprintf( stdout, "FALS count: %d\n", count);
        return -1;
    }
    count++;
    if( !queryParamsTest( "https://secure.equidam.dev/report/index.php?uid=100&cid=100&t=ahrgF4BEtQevm3ZzkpJ5YNUxBS7kRKkr&pdf=true", "100-100-", true )){
        fprintf( stdout, "FALS count: %d\n", count);
        return -1;
    }
    count++;
    
    return 0;
}

bool queryParamsTest( char * url , char * cmp , bool expectedfound){
    int qlen = 100;
    char query[qlen];
    bool found;
    memset( query, '\0', sizeof(char)*qlen );
    found = getQueryParams( url , query, qlen );
    if( strlen( cmp ) != strlen( query ) || strcmp(cmp, query) != 0 || found != expectedfound){
        fprintf( stdout, "not same query: %s, cmp: %s, url: %s\n", query, cmp, url);
        return false;
    }
    time_t current_time;
    int sizeofname = 300;
    char outstr[sizeofname];
    current_time = time(NULL);
    char s[20]; /* strlen("2009-08-10 18:17:54") + 1 */
    strftime(s, 20, "%Y-%m-%d-%H:%M:%S", localtime(&current_time));
    memset( &outstr , 0 , sizeof(outstr));
    snprintf( outstr, sizeofname, "EquidamReport-%s-%s-%d.pdf", query, s, getpid());
    fprintf( stdout, "filename: %s\n", outstr );
    return true;
}
