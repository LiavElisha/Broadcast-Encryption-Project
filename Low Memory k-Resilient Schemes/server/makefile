a.out: server.o aes.o
	gcc -g -o a.out server.o aes.o  -lssl -lcrypto

server.o: server.c
	gcc -c server.c

aes.o: aes.c aes.h
	gcc -c aes.c -lssl -lcrypto
