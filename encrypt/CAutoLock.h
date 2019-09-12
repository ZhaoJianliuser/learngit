#ifndef CAUTO_LOCK_H_
#define CAUTO_LOCK_H_

#include<WinSock2.h>

class CAutoLock
{
public:
	CAutoLock();
	~CAutoLock();

	void Lock();
	void UnLock();

private:
	CRITICAL_SECTION m_Section;
};

#endif
