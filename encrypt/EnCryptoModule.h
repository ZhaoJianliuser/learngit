#ifndef ENCRYPTOMODULE_H_
#define ENCRYPTOMODULE_H_


#include<map>

#include"Cache.h"
#include"common.h"
#include"CAutoLock.h"


typedef struct _NET_PACKET_HEADER {
	unsigned int pw_length;
	unsigned int dw_length;
	int first_byte[8];
}NET_PACKET_HEADER, *PNET_PACKET_HEADER;

struct encrpto_sock_st {
	
	Cache *cache;

	char *sockbuffer;
	char *pwsockbuffer;
	char *dwsockbuffer;
};

__declspec(dllexport) int Encrypto_appending(SOCKET fd);

//�ɹ�:0 ʧ��:-1
__declspec(dllexport) int Encrypto_set_sock_st(SOCKET fd);

__declspec(dllexport) void Encrypto_free(SOCKET fd);

//ִ�гɹ�:���ض�ȡ���ֽ��� ��������-1:socket���� -2:
__declspec(dllexport) int Encrypto_read(SOCKET fd, char *buf, int size, AES_KEY *key, int flag);

//�Դ������ݴ�С������sizeΪ1024�ֽ�
__declspec(dllexport)int Encrypto_write(SOCKET fd, char *buf, int size, AES_KEY *key);


#endif//ENCRYPTOMODULE_H_
