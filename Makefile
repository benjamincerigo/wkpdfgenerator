# Makefile for the socket programming example

CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lwkhtmltox -Wall -ansi -pedantic -ggdb

BIN = bin
vpath %.cpp src
vpath %.cpp lib
SOURCE_FILES = Socket.cpp ServerSocket.cpp ClientSocket.cpp simple_server_main.cpp simple_client_main.cpp BufferedLineReader.cpp SocketException.cpp signal.cpp error.cpp Url.cpp pdf.cpp

simple_server_objects = bin/obj/ServerSocket.o bin/obj/Socket.o bin/obj/BufferedLineReader.o bin/obj/simple_server_main.o bin/obj/SocketException.o bin/obj/signal.o bin/obj/error.o bin/obj/Url.o bin/obj/pdf.o /usr/local/lib/libwkhtmltox.so.0.12.2
simple_client_objects = bin/obj/ClientSocket.o bin/obj/Socket.o bin/obj/simple_client_main.o bin/obj/error.o
OBJ=$(BIN)/obj

OBJECT_FILES     = $(SOURCE_FILES:%.cpp=$(OBJ)/%.o)
# ^^^ A more succinct expression for $(OBJECT_FILES), using
#     http://www.gnu.org/software/make/manual/make.html#Substitution-Refs

build: $(OBJECT_FILES) server client

server: $(simple_server_objects)
	$(CC) $(LDFLAGS) -o $(BIN)/$@ $(simple_server_objects)


client: $(simple_client_objects)
	$(CC) $(LDFLAGS) -o $(BIN)/$@ $(simple_client_objects)

clean:
	rm -rf $(BIN)
    

# http://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: build clean

$(EXECUTABLE_FILES): $(OBJECT_FILES)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build successful!"

# http://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(OBJECT_FILES): $(OBJ)/%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $<
