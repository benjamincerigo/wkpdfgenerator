CC=gcc
PROGS= client \
		server

all: ${PROGS}

client: src/client.c 
	gcc src/client.c -o bin/client

server: src/server-1.c
	gcc src/server-1.c -o bin/server

clean:
	rm bin/*




