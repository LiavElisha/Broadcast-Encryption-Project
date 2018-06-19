a.out: client.o aes.o
	gcc -g -o a.out client.o aes.o  -lssl -lcrypto

client.o: client.c
	gcc -c client.c

aes.o: aes.c aes.h
	gcc -c aes.c -lssl -lcrypto
