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
#include "functions.c"
#include "keys.h"

#ifndef N
#define N 10
#endif

#define L 7
#define CLIENT_INDEX 8
#define KEY_LENGTH 16
#define FILE_NAME "file.txt"
#define ENCRYPTED_FILE_NAME "files/sample_file.enc"

unsigned int RESILIENT[N] = {0};

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
    }
}



void sendFuctionIndexAndSchemeIndexToServer(int index, int sockfd)
{
	    char sendBuff[256];
	    memset(sendBuff, '0', sizeof(sendBuff));

		sendBuff[0] = index + '0';
		sendBuff[1] = f(index, CLIENT_INDEX) + '0';
		sendBuff[2] = '\0';

	    write(sockfd, sendBuff, 3);  
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
            if (feof(fp))
                printf("End of file\n");
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
	}
	fclose(fp);			
}


int client()
{
    unsigned char key[17];
	char fileName[] = "files/ .txt";
	int sockfd = 0;
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
    /* Recieve L files from server */
    for (int i=1; i<= L; ++i)
    {
		sendFuctionIndexAndSchemeIndexToServer(i, sockfd);
		recieveEncryptedDataToFile(sockfd);
		fileName[6] = i + '0';
		createKey(key, KEYS[i - 1]);
		fdecrypt(ENCRYPTED_FILE_NAME, fileName, (unsigned const char*)key);
	    write(sockfd, "OK", 3);    
    }

}

