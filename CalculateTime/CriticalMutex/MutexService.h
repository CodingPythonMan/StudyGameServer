#pragma once
#include <windows.h>
#include <memory>
#include <process.h>
#include <mutex>

#pragma comment(lib, "winmm.lib")

class MutexService
{
public:
	std::mutex m_lock;

	MutexService();
	virtual ~MutexService();

	void Start();

	static unsigned int WINAPI MyThread(LPVOID param);
};

