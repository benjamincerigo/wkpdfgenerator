CC=gcc
PROGS= client \
		server

all: ${PROGS}

client: src/client.c 
	gcc src/client.c -o bin/client

server: src/server.c
	gcc src/server.c -o bin/server

clean:
	rm bin/*




