a.out: client.o aes.o sha256.o
	gcc -g -o a.out client.o aes.o sha256.o -lssl -lcrypto

client.o: client.c
	gcc -c client.c

sha256.o: sha256.c sha256.h
	gcc -c sha256.c -lssl -lcrypto

aes.o: aes.c aes.h
	gcc -c aes.c -lssl -lcrypto



