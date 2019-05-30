CC=gcc
CFLAGS=-I.

client : libserver.o libserver.h
	$(CC) -pthread -o client client.c $(CFLAGS)

server : libserver.o libserver.h
	$(CC) -pthread -o server server.c $(CFLAGS)
	
libserver.o : libserver.h
	$(CC) -o libserver.o -c libserver.c $(CFLAGS)
	
