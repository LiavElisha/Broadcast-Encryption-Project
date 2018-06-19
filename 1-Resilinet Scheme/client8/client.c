//
// Created by liav on 13/12/17.
//


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "aes.h"
#include <time.h>

#define N 10
#define KEY_LENGTH 16
#define FILE_NAME "files/file.txt"
#define ENCRYPTED_FILE_NAME "files/sample_file.enc"

unsigned int RESILIENT[N] = {0};
unsigned const char* KEYS[N] = {"1234567812345670", "1234567812345671", "1234567812345672", "1234567812345673", "1234567812345674",
                                "1234567812345675", "1234567812345676", "1234567812345677", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "1234567812345679" };
int main(int argc, char **argv[])
{
    if (argc < 2)
    {
        printf("Number of client is 0\n");
        return 0;
    }

	setResilientByInput(argc - 1, argv);
	client();

    return 0;
}

int client()
{
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
	unsigned char key[17];

    memset(recvBuff, '0', sizeof(recvBuff));
	memset(key, '0', sizeof(key));
    struct sockaddr_in serv_addr;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

	recieveEncryptedDataToFile(sockfd);
	createKey(key, KEYS);
    fdecrypt(ENCRYPTED_FILE_NAME, FILE_NAME, (unsigned const char*)key);
}


void recieveEncryptedDataToFile(int sockfd)
{
    FILE *fp;
	int bytesReceived = 0;
    char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff)); 
   		
    /* Create file where data will be stored */
	fp = fopen(ENCRYPTED_FILE_NAME, "w");
	if(fp == NULL)
	{
	    printf("Error opening file");
	    return 1;
	}   		
	
	/* Receive data in chunks of 256 bytes */
	while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
	{	
		if(bytesReceived < 0)
		{
			printf("\n Read Error \n");
		}				
		fwrite(recvBuff, 1,bytesReceived,fp);
        /*
         * There is something tricky going on with read ..
         * Either there was error, or we reached end of file.
         */
        if (bytesReceived < 256)
        {
            break;
        }
	}
	fclose(fp);			
}

int createKey(char* key, unsigned const char* keys[])
{
	memset(key, '\0', KEY_LENGTH);  
    for (int i=0; i < N; ++i)
    {
		if(RESILIENT[i])
		{
			
			for (int j=0; j < KEY_LENGTH; ++j)
		    {
		        key[j] = key[j] ^ keys[i][j];
		    }
		}
    }
	
}

void setResilientByInput(int resilientsNumber, char **argv[])
{
    char c;
    int index;

    for (int i=1; i <= resilientsNumber; ++i)
    {
        c = (char)argv[i][0];
        index = c - '0';
		RESILIENT[index]=1;
		printf("resilient += %d\n", index);
    }
}









