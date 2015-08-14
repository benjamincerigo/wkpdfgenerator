#include <iostream>
#include <syslog.h> 
#include <cstdlib>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdio.h>
#include "../src/SocketTimeOut.h"

#ifndef MAXLINE
#define MAXLINE 2000
#endif
#ifndef MAXHOSTNAME
#define MAXHOSTNAME 200
#endif
#ifndef MAXCONNECTIONS
#define MAXCONNECTIONS 5
#endif
#ifndef MAXRECV
#define MAXRECV 500
#endif
void     err_dump(const char *, ...);
void     err_msg(const char *, ...);
void     err_quit(const char *, ...);
void     err_ret(const char *, ...);
void     err_sys(const char *, ...);

typedef void Sigfunc(int);
Sigfunc *Signal(int signo, Sigfunc *func);