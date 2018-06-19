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


int stringToInt(char* buff, int* index);

int main(int argc, char **argv[])
{
	// the client except to get the number of files.
	if (argc != 2)
	{
		printf("argc has to be 2\n");
		return 0;
	}
	char c = argv[1][0];
	int numOfFiles = c - '0';

    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;



	printf("\n TEST1 : numOfFiles - %d \n", numOfFiles);

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


    /* Create file where data will be stored */
    FILE *fp;
    int sizeOfFile;
    int bytesReceivedTotal = 0;
    char fileName[] = " .txt";
    
    
    int index = 0;
    int j;
    for (int i=0; i< numOfFiles; ++i)
    {
    
   	    memset(recvBuff, '0', sizeof(recvBuff));
		printf("\n ///////////////////////////////////////////////////////////////////////////// \n");	
    		
 		fp = fopen("sample_file.enc", "w");
		if(fp == NULL)
		{
		    printf("Error opening file");
		    return 1;
		}   		
    		
    	printf("test1\n");
		bytesReceived = read(sockfd, recvBuff, 256);
		printf("test2\n");
		if(bytesReceived < 0)
		{
		    printf("\n Read Error \n");
		}	
		sizeOfFile = stringToInt(recvBuff, &index);
		
		fwrite(recvBuff + index, 1,bytesReceived - index,fp);
		printf("recvBuff - %s%S \n", recvBuff, recvBuff+index);
		
		
		
		if (sizeOfFile > bytesReceived - index)
		{
			/* Receive data in chunks of 256 bytes */
			while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
			{		
				if(bytesReceived < 0)
				{
					printf("\n Read Error \n");
				}				
				printf("Bytes received %d\n",bytesReceived);
				// recvBuff[n] = 0;
				fwrite(recvBuff, 1,bytesReceived,fp);
				printf("%s \n", recvBuff);		    
			}				
		}

		
			fileName[0] = i+ '0';
			fclose(fp);
			fdecrypt("sample_file.enc", fileName, (unsigned const char*)"1234567812345678");
		    write(sockfd, "OK", 3);    
    }
    return 0;
}





int stringToInt(char* buff, int* index)
{
	int i =0;
	int sum = 0;
	while(buff[i] != '\0')
	{
		sum = 10*sum + buff[i] - '0';
		++i;
	}
	*index = i+1;

	return sum;
}







