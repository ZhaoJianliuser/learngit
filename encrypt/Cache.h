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

	//得到缓存中当前数据量
	int getsize();
	//插入数据pdata:数据指针 len:数据大小 返回:成功0 失败-1
	int insert(char *pdata, int len);
	//取出数据data:数据缓存指针 len:取数据大小 返回:成功0 失败-1
	int popup(char *data, int len);

private:
	//判断缓存是否已满
	bool b_full;

	//插入指针
	char *in_ptr;
	//取数据指针
	char *ou_ptr;
	//缓存尾指针
	char *end_pointer;

private:
	//缓存
	char *cache_buffer;
};


#endif