//
// Created by liav on 24/03/18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "aes.h"
#include <time.h>
#include "keys.h"

#define N 10
#define L 7
#define KEY_LENGTH 16
#define FILE_NAME "file.enc"



unsigned int RESILIENT[N] = {0};

unsigned int CREATED_FILES[7][8] = {0};


int main(int argc, char **argv[])
{
    if (argc < 2)
    {
        printf("Number of client is 0\n");
        return -1;
    }

	setResilientByInput(argc - 1, argv);
	server();
	
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



void encryptFile(char* fileToEncrypt, char* ciphertextFileName, unsigned const char* keys[])
{
    unsigned char key[17];
    memset(key, '\0', sizeof(key));
   	createKey(key, keys);
	fencrypt(fileToEncrypt, ciphertextFileName, (unsigned const char*)key);
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



void updatefileName(char* fileName, int firstIndex, char a, int secondIndex, char b)
{
    fileName[firstIndex] = a;
	if(secondIndex >= 0)
	{
		fileName[secondIndex] = b;
	}
}



int recieveSchemeDetailsFromClient(int connfd, int* functionIndex, int* schemeIndex)
{
	int bytesReceived = 0;
	char recvBuff[256];
	memset(recvBuff, '0', sizeof(recvBuff));

	// Get functionIndex & schemeIndex from user
	bytesReceived = read(connfd, recvBuff, 256);
    if(bytesReceived < 0)
    {
        printf("\n Read Error in recieveSchemeDetailsFromClient function \n");
		return -1;
    }
    *functionIndex = recvBuff[0] - '0';
    *schemeIndex = recvBuff[1] - '0';
	
	return 1;
}


int sendDataToClient(int connfd, FILE *fp)
{
    unsigned char buff[256];
    memset(buff, '0', sizeof(buff));
	int nread;

    while(1)
    {
        printf("Read data from file and send it \n");

        /* First read file in chunks of 256 bytes */
        memset(buff, '0', sizeof(buff));
        nread = fread(buff,1,256,fp);

        /* If read was success, send data. */
        if(nread > 0)
        {
            write(connfd, buff, nread);
        }

        /*
         * There is something tricky going on with read ..
         * Either there was error, or we reached end of file.
         */
        if (nread < 256)
        {
            if (feof(fp))
                printf("End of file\n");
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
    }
}


int recieveOKFromClient(int connfd)
{
	int bytesReceived = 0;
	char recvBuff[256];
	memset(recvBuff, '0', sizeof(recvBuff));

	bytesReceived = read(connfd, recvBuff, 3);
    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
		return -1;
    }

}

void server()
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
	char recvBuff[1025];
    FILE *fp;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }
    
    unsigned char buff[256];
    memset(buff, '0', sizeof(buff));
	
	int functionIndex = 0;
	int schemeIndex = 0;
	char fileToEncrypt[] = "dataFiles/fileToEncrypt0.txt";
	char encryptedFileToSend[] = "encryptedFiles/file00.enc";

	int bytesReceived = 0;

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        for (int i=0; i < L; ++i)
        {			
			if (! recieveSchemeDetailsFromClient(connfd, &functionIndex, &schemeIndex))
			{
				return -1;
			}
			updatefileName(fileToEncrypt, 23, functionIndex + '0', -1, '0');
			updatefileName(encryptedFileToSend, 19, functionIndex + '0', 20, schemeIndex + '0');

		    if(!CREATED_FILES[functionIndex - 1][schemeIndex])
		    {
		        printf("encrypt the file \n");
			    encryptFile(fileToEncrypt, encryptedFileToSend, KEYS[functionIndex - 1][schemeIndex]);
		    }

            fp = fopen(encryptedFileToSend,"rb");
            if(fp==NULL)
            {
                printf("File opern error. file name - %s\n", encryptedFileToSend);
                return 1;
            }

            /* Read data from file and send it */
			sendDataToClient(connfd, fp);

			if (!recieveOKFromClient(connfd))
			{
				return -1;
			}
        }
    close(connfd);
    sleep(1);
    }
}



