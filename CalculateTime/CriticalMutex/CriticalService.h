#pragma once
#include <windows.h>
#include <memory>
#include <process.h>

#pragma comment(lib, "winmm.lib")

class CriticalService
{
public:
	CRITICAL_SECTION m_lock;

	CriticalService();
	virtual ~CriticalService();

	void Start();

	static unsigned int WINAPI MyThread(LPVOID param);
};

