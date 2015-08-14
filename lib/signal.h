typedef void Sigfunc(int);
Sigfunc * Signal(int signo, Sigfunc *func);
