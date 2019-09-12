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

//成功:0 失败:-1
__declspec(dllexport) int Encrypto_set_sock_st(SOCKET fd);

__declspec(dllexport) void Encrypto_free(SOCKET fd);

//执行成功:返回读取的字节数 参数错误-1:socket错误 -2:
__declspec(dllexport) int Encrypto_read(SOCKET fd, char *buf, int size, AES_KEY *key, int flag);

//对传送数据大小的限制size为1024字节
__declspec(dllexport)int Encrypto_write(SOCKET fd, char *buf, int size, AES_KEY *key);


#endif//ENCRYPTOMODULE_H_
