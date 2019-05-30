CC=gcc
CFLAGS=-I.

tcpclient :
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)

tcpserver : 
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)
	
udpclient : 
	$(CC) -o udpclient udpclient.c $(CFLAGS)

udpserver : 
	$(CC) -o udpserver udpserver.c $(CFLAGS)

all : tcpclient tcpserver udpclient udpserver
udp : udpclient udpserver
tcp : tcpclient tcpserver
	
