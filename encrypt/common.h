#ifndef COMMON_H__
#define COMMON_H__

#include<assert.h>

#include<openssl/aes.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<openssl/bio.h>


#define AES_MSG_WAITALL		0x10
#define AES_MSG_DONTWAIT	0x20

#define MAX_BUFFER_LENGTH   1024
#define MAX_SOCKBUFFER_LEN  (5 * MAX_BUFFER_LENGTH)


#endif
