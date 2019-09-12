#include"Cache.h"


Cache::Cache()
{
	cache_buffer = (char *)malloc(CACHESIZE);
	if (cache_buffer == NULL) {

		assert(false);
		return;
	}

	memset(cache_buffer, 0, CACHESIZE);

	b_full = false;

	in_ptr = cache_buffer;
	ou_ptr = cache_buffer;
	end_pointer = cache_buffer + (CACHESIZE - 1);
}

Cache::~Cache()
{
	if (cache_buffer != NULL)
		free(cache_buffer);
}

int Cache::getsize()
{
	int data_size;

	if (b_full) {
		data_size = CACHESIZE;
	}
	else {
		data_size = in_ptr - ou_ptr >= 0 ? in_ptr - ou_ptr : CACHESIZE - (ou_ptr - in_ptr);
	}

	return data_size;
}

int Cache::insert(char *pdata, int len)
{
	assert(pdata && len > 0);

	if (b_full) {
		return -1;
	}

	int all_left, right_left;

	all_left = ou_ptr - in_ptr > 0 ? ou_ptr - in_ptr : CACHESIZE - (in_ptr - ou_ptr);
	if (len > all_left) {
		return -1;
	}

	right_left = ou_ptr - in_ptr > 0 ? ou_ptr - in_ptr : end_pointer - in_ptr + 1;

	if (len > right_left) {
		memcpy(in_ptr, pdata, right_left);
		in_ptr = cache_buffer;
		memcpy(in_ptr, pdata + right_left, len - right_left);
		in_ptr += (len - right_left);
	}
	else if (len == right_left) 
	{
		if (in_ptr - ou_ptr > 0) {
			memcpy(in_ptr, pdata, right_left);
			in_ptr = cache_buffer;
		}else {
			memcpy(in_ptr, pdata, right_left);
			in_ptr += right_left;
		}
	}else {
		memcpy(in_ptr, pdata, len);
		in_ptr += len;
	}

	if (in_ptr == ou_ptr)
		b_full = true;

	return 0;
}

int Cache::popup(char *data, int len)
{
	assert(data);
	assert(len > 0);

	int data_size, right_size;

	if (b_full) {
		data_size = CACHESIZE;
	}
	else {
		data_size = in_ptr - ou_ptr >= 0 ? in_ptr - ou_ptr : CACHESIZE - (ou_ptr - in_ptr);
	}

	if (len > data_size) {
		return -1;
	}

	right_size = in_ptr - ou_ptr > 0 ? in_ptr - ou_ptr : end_pointer - ou_ptr + 1;

	if (len > right_size) {
		memcpy(data, ou_ptr, right_size);
		ou_ptr = cache_buffer;
		memcpy(data + right_size, ou_ptr, len - right_size);
		ou_ptr += (len - right_size);
	}
	else if (len == right_size) 
	{
		if (ou_ptr - in_ptr > 0) {
			memcpy(data, ou_ptr, right_size);
			ou_ptr = cache_buffer;
		}else {
			memcpy(data, ou_ptr, right_size);
			ou_ptr += right_size;
		}
	}else {
		memcpy(data, ou_ptr, len);
		ou_ptr += len;
	}

	b_full = false;

	return 0;
}
