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

#define ENCRYPTED_FILE_NAME "files/sample_file.enc"
#define N 10

void client();
void recieveEncryptedDataToFile(int sockfd);


unsigned const char* KEY = "1234567812345679";
int numOfFiles;

int main(int argc, char **argv[])
{

	// the client expect to get the number of files.
	if (argc < 2)
	{
		printf("argc has to be at least  2\n");
		return 0;
	}

	numOfFiles = N - (argc - 1);
	client();

    return 0;
}


void client()
{
	int sockfd = 0;
    struct sockaddr_in serv_addr;
	char fileName[] = "files/ .txt";

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
    
    for (int i=0; i < numOfFiles; ++i)
    {
		recieveEncryptedDataToFile(sockfd);	

		fileName[6] = i+ '0';
		fdecrypt(ENCRYPTED_FILE_NAME, fileName, (unsigned const char*)KEY);
	    write(sockfd, "OK", 3);    
    }
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
		
        if (bytesReceived < 256)
        {
            break;
        }
	}
	fclose(fp);			
}


