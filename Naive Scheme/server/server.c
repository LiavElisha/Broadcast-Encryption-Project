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
#define N 10

unsigned const char* KEYS[N] = {"1234567812345670", "1234567812345671", "1234567812345672", "1234567812345673", "1234567812345674",
                                "1234567812345675", "1234567812345676", "1234567812345677", "1234567812345678", "1234567812345679" };

unsigned int RESILIENT[N] = {0};
int main(int argc, char **argv[])
{
    if (argc < 2)
    {
        printf("Number of client is 0\n");
        return 0;
    }

	setResilientByInput(argc - 1, argv);
	encryptFiles();
	server();
    return 0;
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



void encryptFiles()
{
	char fileToEncrypt[] = "dataFiles/fileToEncrypt.txt";
	char encryptedFile[] = "encryptedFiles/file0.enc";

    for(int i = 0; i < N; ++i)
    {
		if(RESILIENT[i])
			continue;
		encryptedFile[19] = i + '0';
        fencrypt(fileToEncrypt, encryptedFile, (unsigned const char*)KEYS[(i)]);
    }

}


int sendFile(int index, int connfd)
{
	char sendBuff[256];
	char fileToSend[] = "encryptedFiles/file0.enc";
	int nread;
	FILE *fp;

	fileToSend[19] = index + '0';
	/* Open the file that we wish to send */
    fp = fopen(fileToSend, "rb");
    if(fp==NULL)
    {
        printf("sendFile function --> File open error");
        return -1;
    }

	 while(1)
	{
		/* First read file in chunks of 256 bytes */
		memset(sendBuff, '0', sizeof(sendBuff));
		nread = fread(sendBuff, 1, 256, fp);

		/* If read was success, send data. */
		if(nread > 0)
		{
		    write(connfd, sendBuff, nread);
		}
		
		/*
		 * There is something tricky going on with read ..
		 * Either there was error, or we reached end of file.
		 */
		if (nread < 256)
		{
			if(nread == 0)
			{
				write(connfd, "\0", 1);
			}

		    if (feof(fp))
		        // printf("End of file\n");
		    if (ferror(fp))
		        printf("Error reading\n");
		    break;
		}
	}
	fclose(fp);
}

int recieveOKFromClient(int connfd)
{
	int bytesReceived = 0;
	char recvBuff[256];
	memset(recvBuff, '0', sizeof(recvBuff));

	bytesReceived = read(connfd, recvBuff, 256);
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

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        for(int i=0 ; i < N; ++i)
        {
			if(RESILIENT[i])
				continue;
			sendFile(i, connfd);
			recieveOKFromClient(connfd);
        }
    	close(connfd);
    	sleep(1);
    }
}



