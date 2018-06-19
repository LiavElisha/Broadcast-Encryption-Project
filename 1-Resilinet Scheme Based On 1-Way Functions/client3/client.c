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


#include "sha256.h"

#define N 8
#define KEY_LENGTH 16
#define FILE_NAME "files/file.txt"
#define ENCRYPTED_FILE_NAME "files/sample_file.enc"
int updateKeys(BYTE* key, int depth, int* index);
int getKeys(BYTE* key, BYTE* o_key1, BYTE* o_key2);
void printKeys();

unsigned int RESILIENT[N] = {0};
unsigned char KEYS[N][17] = { "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                                 "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                                 "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};

BYTE GIVEN_KEYS[3][17] = {{63, 42, 212, 216, 244, 118, 127, 61, 72, 189, 226, 204, 47, 114, 186, 208},
                        {177, 10, 104, 147, 213, 156, 0, 144, 142, 166, 71, 68, 157, 58, 23, 167},
                        {49, 198, 197, 162, 101, 158, 139, 122, 43, 152, 75, 108, 173, 195, 11, 22}
};




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

    unsigned char key[17];
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff));
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
	// printf("strlen(text) - %d\n", strlen(text));
    
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
	updateKeys(GIVEN_KEYS[0], 1, &index);
	updateKeys(GIVEN_KEYS[1], 0, &index);
	++index;
	updateKeys(GIVEN_KEYS[2], 2, &index);

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



