#ifndef CACHE_H_
#define CACHE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"common.h"

#define CACHESIZE	(4 * 1024)//4k

class Cache
{
public:
	Cache();
	~Cache();

	//�õ������е�ǰ������
	int getsize();
	//��������pdata:����ָ�� len:���ݴ�С ����:�ɹ�0 ʧ��-1
	int insert(char *pdata, int len);
	//ȡ������data:���ݻ���ָ�� len:ȡ���ݴ�С ����:�ɹ�0 ʧ��-1
	int popup(char *data, int len);

private:
	//�жϻ����Ƿ�����
	bool b_full;

	//����ָ��
	char *in_ptr;
	//ȡ����ָ��
	char *ou_ptr;
	//����βָ��
	char *end_pointer;

private:
	//����
	char *cache_buffer;
};


#endif