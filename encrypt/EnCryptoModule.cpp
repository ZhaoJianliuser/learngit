#include"EnCryptoModule.h"


#pragma comment(lib, "ws2_32.lib")


//这里又做了多次拷贝，有点小号性能啊

CAutoLock mapLock;
static std::map<SOCKET, encrpto_sock_st> encrypto_map;//多线程条件下对map的操作需要加锁，这一点需要非常小心


int 
Encrypto_appending(SOCKET fd)
{
	int left = 0;
	//加锁
	mapLock.Lock();

	std::map<SOCKET, encrpto_sock_st>::iterator it = encrypto_map.find(fd);
	encrpto_sock_st ess = it->second;

	left = ess.cache->getsize();
	//释放锁
	mapLock.UnLock();

	return left;
}

//对套接字绑定缓存
int
Encrypto_set_sock_st(SOCKET fd)
{
	encrpto_sock_st ess = { 0 };

	ess.cache = new Cache();
	if (NULL == ess.cache) {
		goto error;
	}

	ess.sockbuffer = (char *)malloc(MAX_SOCKBUFFER_LEN);
	if (NULL == ess.sockbuffer) {
		goto error1;
	}
	ess.dwsockbuffer = (char *)malloc(MAX_SOCKBUFFER_LEN);
	if (NULL == ess.dwsockbuffer) {
		goto error2;
	}
	ess.pwsockbuffer = (char *)malloc(MAX_SOCKBUFFER_LEN);
	if (NULL == ess.pwsockbuffer) {
		goto error3;
	}
	goto succeed;

error3:
	free(ess.dwsockbuffer);
error2:
	free(ess.sockbuffer);
error1:
	delete ess.cache;
error:
	return -1;

succeed:

	memset(ess.sockbuffer,   0, MAX_SOCKBUFFER_LEN);
	memset(ess.dwsockbuffer, 0, MAX_SOCKBUFFER_LEN);
	memset(ess.pwsockbuffer, 0, MAX_SOCKBUFFER_LEN);

	mapLock.Lock();
	encrypto_map.insert(std::pair<SOCKET, encrpto_sock_st>(fd, ess));
	mapLock.UnLock();

	return 0;
}

//释放与套接字绑定的缓存
void
Encrypto_free(SOCKET fd)
{
	//加锁
	mapLock.Lock();

	std::map<SOCKET, encrpto_sock_st>::iterator it = encrypto_map.find(fd);
	encrpto_sock_st ess = it->second;

	delete ess.cache;

	free(ess.sockbuffer);
	free(ess.dwsockbuffer);
	free(ess.pwsockbuffer);

	encrypto_map.erase(it);
	//释放锁
	mapLock.UnLock();
}

int
Encrypto_read(SOCKET fd, char *buf, int size, AES_KEY *key, int flag)
{
	assert(buf && key && size > 0);
	assert(AES_MSG_WAITALL == flag || AES_MSG_DONTWAIT == flag);

	int ret, min;
	int data_size, left_len = size;
	NET_PACKET_HEADER net_packet_header = { 0 };
	encrpto_sock_st ess;

	int dw_times;
	char dw_buffer[AES_BLOCK_SIZE];

	//加锁
	mapLock.Lock();
	std::map<SOCKET, encrpto_sock_st>::iterator it = encrypto_map.find(fd);
	ess = it->second;
	mapLock.UnLock();
	
	data_size = ess.cache->getsize();
	if (size <= data_size) {
		ess.cache->popup(buf, size);
		return size;
	}
	
	if (flag == AES_MSG_DONTWAIT) { 
		if (data_size > 0) {
			min = data_size > size ? size : data_size;
			ess.cache->popup(buf, min);
			return min;
		}
	}
	do
	{
		ret = recv(fd, (char *)&net_packet_header, sizeof(net_packet_header), MSG_WAITALL);
		if (ret <= 0) {
			return -1;
		}

		memset(ess.sockbuffer, 0, sizeof(ess.sockbuffer));
		ret = recv(fd, ess.sockbuffer, net_packet_header.pw_length, MSG_WAITALL);
		if (ret <= 0) {
			return -1;
		}
		//解密
		memset(ess.dwsockbuffer, 0, sizeof(ess.dwsockbuffer));
		dw_times = net_packet_header.pw_length / AES_BLOCK_SIZE;

		for (int i = 0; i < dw_times; i++){
			memset(dw_buffer, 0, AES_BLOCK_SIZE);
			AES_decrypt((unsigned char *)ess.sockbuffer + i * AES_BLOCK_SIZE, (unsigned char *)dw_buffer, key);
			memcpy(ess.dwsockbuffer + i * AES_BLOCK_SIZE, dw_buffer, AES_BLOCK_SIZE);
		}

		ess.cache->insert(ess.dwsockbuffer, net_packet_header.dw_length);
		data_size = ess.cache->getsize();
		
		if (AES_MSG_WAITALL == flag) {
			if (data_size >= left_len) {
				ess.cache->popup(buf + (size - left_len), left_len);
				return size;
			}else {
				ess.cache->popup(buf + (size - left_len), data_size);
				left_len -= data_size;
			}
		}else if (AES_MSG_DONTWAIT == flag) {
			min = data_size > size ? size : data_size;
			ess.cache->popup(buf, min);
			return min;
		}
	} while (true);
}

int
Encrypto_write(SOCKET fd, char *buf, int size, AES_KEY *key)
{
	assert(buf && key && size > 0);

	int pw_times, send_length;
	NET_PACKET_HEADER net_packet_header = { 0 };
	encrpto_sock_st ess;

	char pw_buffer[AES_BLOCK_SIZE];

	mapLock.Lock();
	std::map<SOCKET, encrpto_sock_st>::iterator it = encrypto_map.find(fd);
	ess = it->second;
	mapLock.UnLock();

	memset(ess.sockbuffer, 0, sizeof(ess.sockbuffer));
	memset(ess.pwsockbuffer, 0, sizeof(ess.pwsockbuffer));

	pw_times = (size - 1) / AES_BLOCK_SIZE + 1;

	for (int i = 0; i < pw_times; i++){
		memset(pw_buffer, 0, AES_BLOCK_SIZE);
		AES_encrypt((unsigned char *)buf + i * AES_BLOCK_SIZE, (unsigned char *)pw_buffer, key);
		memcpy(ess.pwsockbuffer + sizeof(NET_PACKET_HEADER) + i * AES_BLOCK_SIZE, pw_buffer, AES_BLOCK_SIZE);
	}

	net_packet_header.pw_length = pw_times * AES_BLOCK_SIZE;
	net_packet_header.dw_length = size;
	memcpy(ess.pwsockbuffer, &net_packet_header, sizeof(net_packet_header));

	send_length = net_packet_header.pw_length + sizeof(net_packet_header);

	if (SOCKET_ERROR == send(fd, ess.pwsockbuffer, send_length, 0)) {
		return -1;
	}
	return 0;
}