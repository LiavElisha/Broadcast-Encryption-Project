//
// Created by liav on 13/12/17.
//

#ifndef UNTITLED3_AES_H
#define UNTITLED3_AES_H

#endif //UNTITLED3_AES_H



#include <openssl/aes.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>


struct ctr_state
{
    unsigned char ivec[AES_BLOCK_SIZE];
    unsigned int num;
    unsigned char ecount[AES_BLOCK_SIZE];
};


FILE *readFile;
FILE *writeFile;
AES_KEY key;

int bytes_read, bytes_written;
unsigned char indata[AES_BLOCK_SIZE];
unsigned char outdata[AES_BLOCK_SIZE];
unsigned char iv[AES_BLOCK_SIZE];
struct ctr_state state;

int init_ctr(struct ctr_state *state, const unsigned char iv[16]);

void fencrypt(char* read, char* write, const unsigned char* enc_key);

void fdecrypt(char* read, char* write, const unsigned char* enc_key);