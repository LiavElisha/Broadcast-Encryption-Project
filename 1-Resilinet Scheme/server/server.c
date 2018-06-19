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
#define KEY_LENGTH 16
#define FILE_NAME "encryptedFiles/file.enc"

unsigned int RESILIENT[N] = {0};
unsigned const char* KEYS[N] = {"1234567812345670", "1234567812345671", "1234567812345672", "1234567812345673", "1234567812345674",
                                "1234567812345675", "1234567812345676", "1234567812345677", "1234567812345678", "1234567812345679" };
int main(int argc, char **argv[])
{
    if (argc < 2)
    {
        printf("Number of client is 0\n");
        return 0;
    }

	setResilientByInput(argc - 1, argv);
	server();

    return 0;
}


int server()
{
	unsigned char key[17];
	
	createKey(key, KEYS);
    fencrypt("dataFiles/fileToEncrypt.txt", FILE_NAME, (unsigned const char*)key);

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
		sendDataFromFile(connfd);
		close(connfd);
		sleep(1);
    }
}


void sendDataFromFile(int connfd)
{
	FILE* fp;
	char sendBuff[256];
	int nread;

    fp = fopen(FILE_NAME,"rb");
    if(fp==NULL)
    {
        printf("File opern error");
        return 1;
    }

    /* Read data from file and send it */
    while(1)
    {
        /* First read file in chunks of 256 bytes */
        memset(sendBuff, '0', sizeof(sendBuff));
        nread = fread(sendBuff, 1, 256, fp);
        printf("Bytes read %d \n", nread);
        printf("%s \n", sendBuff);

        /* If read was success, send data. */
        if(nread > 0)
        {
            printf("Sending \n");
            write(connfd, sendBuff, nread);
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


