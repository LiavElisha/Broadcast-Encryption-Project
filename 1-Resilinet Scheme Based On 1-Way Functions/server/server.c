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
#include "sha256.h"
#include <time.h>

#define N 10
#define KEY_LENGTH 16
#define ENCRYPTED_FILE_NAME "encryptedFiles/file.enc"
#define FILE_NAME "dataFiles/fileToEncrypt.txt"

int updateKeys(BYTE* key, int depth, int* index);
int getKeys(BYTE* key, BYTE* o_key1, BYTE* o_key2);
void printKeys();
unsigned int RESILIENT[N] = {0};
unsigned char KEYS[N][17] = { "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                                 "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                                 "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};

BYTE GIVEN_KEYS[17] = {49, 50, 51, 52, 53, 54, 55, 56, 49, 50, 51, 52, 53, 54, 55, 56};

// unsigned const char* KEYS[N] = {"1234567812345670", "1234567812345671", "1234567812345672", "1234567812345673", "1234567812345674",
//                                 "1234567812345675", "1234567812345676", "1234567812345677", "1234567812345678", "1234567812345679" };
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
    fencrypt(FILE_NAME, ENCRYPTED_FILE_NAME, (unsigned const char*)key);

    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    int numrv;
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

    fp = fopen(ENCRYPTED_FILE_NAME,"rb");
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
        // printf("Bytes read %d \n", nread);
        // printf("%s \n", sendBuff);

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


int updateKeys(BYTE* key, int depth, int* index){
	if(depth == 0){
		memcpy(KEYS[*index], key, 16);
		++(*index);
		return 1;
	}
	
	BYTE key1[] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
	BYTE key2[] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
	
	getKeys(key, key1, key2);

	updateKeys(key1, depth - 1, index);
	updateKeys(key2, depth - 1, index);			
}


int getKeys(BYTE* key, BYTE* o_key1, BYTE* o_key2){
	int i,j;

    BYTE text[32];
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
    // copy the key to hash input
    for (int i = 0; i < 16; ++i){
        text[i] = key[i];
        text[i + 15] = key[i];
    }
	text[31] = '\0';
	// hash(text)
    sha256_init(&ctx);
	sha256_update(&ctx, text, 31);
	sha256_final(&ctx, buf);
    
	// Split the key (hash value) to key1 & key2
	for (int i=0; i<16; ++i)
	{
		o_key1[i] = buf[i];
		o_key2[i] = buf[i+16];
	}

    return 1;
}


// for debug
void printKeys(){
	int i, j;
	for(j = 0; j <= 7; ++j){
		printf("key%d = {", j);
		for(i=0; i<15;++i){
			printf("%d, ", KEYS[j][i]);
		}
		printf("%d} \n", KEYS[j][i]);
	}
}

int createKey(char* key, unsigned char keys[][17])
{
	int index = 0;
	updateKeys(GIVEN_KEYS, 3, &index);
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



