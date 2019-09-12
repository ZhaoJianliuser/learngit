#include "CAutoLock.h"

CAutoLock::CAutoLock()
{
	InitializeCriticalSection(&m_Section);
}

CAutoLock::~CAutoLock()
{
	DeleteCriticalSection(&m_Section);
}

void CAutoLock::Lock()
{
	EnterCriticalSection(&m_Section);
}

void CAutoLock::UnLock()
{
	LeaveCriticalSection(&m_Section);
}