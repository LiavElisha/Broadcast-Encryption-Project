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

#define N 10

unsigned const char* KEYS[N] = {"1234567812345670", "1234567812345671", "1234567812345672", "1234567812345673", "1234567812345674",
                                "1234567812345675", "1234567812345676", "1234567812345677", "1234567812345678", "1234567812345679" };
int main(int argc, char **argv[])
{

    printf("%s\n", KEYS[8]);
    if (argc < 2)
    {
        printf("Number of client is 0\n");
        return 0;
    }

    int stringSize = 5;
    int numOfFile = argc - 1;
    char** fileNames = malloc(numOfFile * sizeof(char *));

    if(fileNames == NULL)
    {
        printf("malloc failed\n");
        return 0;

    }
    for(int i=0; i<numOfFile; ++i)
    {
        fileNames[i] = (char *)malloc(stringSize+1);
        if(fileNames[i] == NULL)
        {
            printf("malloc failed\n");
            return 0;

        }
        char c = (char) argv[i+1][0];
        fileNames[i][0] = c;
        fileNames[i][1] = '.';
        fileNames[i][2] = 'e';
        fileNames[i][3] = 'n';
        fileNames[i][4] = 'c';
        fileNames[i][5] = '\0';
        fencrypt("fileToEncrypt.txt", fileNames[i], (unsigned const char*)KEYS[(c - '0')]);
    }


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

    int sizeOfFile = 0;
    unsigned char buff[256];
    memset(buff, '0', sizeof(buff));

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        /* Open the file that we wish to transfer */
        for(int i=0 ; i< numOfFile; ++i)
        {
            memset(buff, '0', sizeof(buff));
            fp = fopen(fileNames[i],"rb");
            if(fp==NULL)
            {
                printf("File opern error");
                return 1;
            }

            fseek(fp, 0L, SEEK_END);
            int sizeOfFile = ftell(fp);
            fseek(fp, 0L, SEEK_SET);



            int x = sprintf(buff, "%d", sizeOfFile) + 1;
            if (x <=0)
            {
                printf("\nsprintf - failed\n");
            }

            int nread = fread(buff + x,1,256 - x,fp);
            printf("Bytes read %d \n", nread);
            printf("%s \n", buff);

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread + x);
                printf("TETS1 \n");
            }

			printf("TEST2 \n");		

            if ((nread == (256 - x)))
            {
            			printf("TEST3 \n");	
            
                /* Read data from file and send it */
                while(1)
                {
                    /* First read file in chunks of 256 bytes */

                    memset(buff, '0', sizeof(buff));
                    int nread = fread(buff,1,256,fp);
                    printf("Bytes read %d \n", nread);
                    printf("%s \n", buff);

                    /* If read was success, send data. */
                    if(nread > 0)
                    {
                        printf("Sending \n");
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
			printf("TEST7 \n");	
            fclose(fp);
            
            
			printf("TEST4 \n");	
            read(connfd, buff, 3);
            
            
			printf("TEST5 \n");	
        }

    }


    close(connfd);
    sleep(1);



    for(int i=0; i<numOfFile; ++i)
    {
    free(fileNames[i]);
    }
    free(fileNames);


    return 0;
}
