#include <iostream>
#include <atomic>
#include <chrono>
#include <cmath>
#include <thread>
#include <Windows.h>
#include <vector>
#include <mutex>
#include <winnt.h>

#define LOOF_COUNT 1'000'000
#define THREAD_COUNT 10

//#define SLEEP
#define YIELD


int sum1 = 0;
int sum2 = 0;
int sum3 = 0;
int sum4 = 0;
int sum5 = 0;

std::atomic<bool> bACE = false;
std::mutex m_lock;
std::mutex m_lock2;

long lock = 0;
//std::atomic<long> lock(0);
CRITICAL_SECTION cs;
SRWLOCK srwLock;

void AtomicCompare()
{
	bool flag = false;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < LOOF_COUNT;)
	{
		flag = false;
		while (!bACE.compare_exchange_strong(flag, true))
		{
			flag = false;
#ifdef YIELD
			YieldProcessor();
#endif // YIELD

#ifdef SLEEP
			Sleep(0);
#endif // SLEEP
		}
		sum1++;
		i++;
		bACE.store(false);
		//flag = true;
		//bACE.compare_exchange_strong(flag, false);
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	m_lock2.lock();
	std::cout << "compare_exchange_strong - ms :" << ms.count() << "\n" << std::endl;
	m_lock2.unlock();

}


void Interlock()
{

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < LOOF_COUNT;)
	{
		int spinCount = 0;

		while (1 == _InterlockedCompareExchange(&lock, 1, 0))
		{
#ifdef YIELD
			if (spinCount < 100)
			{
				YieldProcessor();
				spinCount++;
			}
			else
			{
				Sleep(1);
			}
			
#endif // YIELD

#ifdef SLEEP
			Sleep(0);
#endif // SLEEP
		}
		sum2++;
		i++;
		lock = 0;
		//_InterlockedCompareExchange(&lock, 0, 1);
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	m_lock2.lock();

	std::cout << "_InterlockedCompareExchange - ms :" << ms.count() << "\n" << std::endl;
	m_lock2.unlock();
}

void mutex()
{

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < LOOF_COUNT;)
	{
		m_lock.lock();
		sum3++;
		i++;
		m_lock.unlock();
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	m_lock2.lock();
	std::cout << "mutex - ms :" << ms.count() << "\n" << std::endl;
	m_lock2.unlock();
}

void CriticalSection()
{

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < LOOF_COUNT;)
	{
		EnterCriticalSection(&cs);
		sum4++;
		i++;
		LeaveCriticalSection(&cs);
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	m_lock2.lock();
	std::cout << "CriticalSection - ms :" << ms.count() << "\n" << std::endl;
	m_lock2.unlock();
}

void SRWLock()
{

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < LOOF_COUNT;)
	{
		AcquireSRWLockExclusive(&srwLock);
		sum5++;
		i++;
		ReleaseSRWLockExclusive(&srwLock);
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	m_lock2.lock();
	std::cout << "SRWLock - ms :" << ms.count() << "\n" << std::endl;
	m_lock2.unlock();
}

int main()
{
	std::vector<std::thread> ths;

	for (int i = 0; i < THREAD_COUNT; i++)
		ths.emplace_back(std::thread(mutex));
	for (auto& th : ths)
		th.join();
	ths.clear();

	for (int i = 0; i < THREAD_COUNT; i++)
		ths.emplace_back(std::thread(AtomicCompare));
	for (auto& th : ths)
		th.join();
	ths.clear();

	for (int i = 0; i < THREAD_COUNT; i++)
		ths.emplace_back(std::thread(Interlock));
	for (auto& th : ths)
		th.join();
	ths.clear();

	InitializeCriticalSection(&cs);
	for (int i = 0; i < THREAD_COUNT; i++)
		ths.emplace_back(std::thread(CriticalSection));
	for (auto& th : ths)
		th.join();
	DeleteCriticalSection(&cs);
	ths.clear();

	InitializeSRWLock(&srwLock);
	for (int i = 0; i < THREAD_COUNT; i++)
		ths.emplace_back(std::thread(SRWLock));
	for (auto& th : ths)
		th.join();

	std::cout << "compare_exchange_strong -> sum :" << sum1 << std::endl;
	std::cout << "_InterlockedCompareExchange -> sum :" << sum2 << std::endl;
	std::cout << "mutex -> sum :" << sum3 << std::endl;
	std::cout << "CriticalSection -> sum :" << sum4 << std::endl;
	std::cout << "SRWLock -> sum :" << sum5 << std::endl;
}