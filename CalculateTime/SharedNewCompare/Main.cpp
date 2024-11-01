#include <iostream>
#include <Windows.h>
#include <memory>
#include <process.h>

#pragma comment(lib, "winmm.lib")

LARGE_INTEGER Freq;

#define SAME_COUNT 1000000


class IInnerData
{
public:
    IInnerData() = default;
    virtual ~IInnerData()
    {
        Release();
        //delete this;
    }

    virtual void Release() {};	// 해제해줘야 하는 항목 있으면 오버라이드
};

class spData : public IInnerData
{
public:
    int X;
    int Y;

    spData() { X = 0; Y = 0; }
};

class B
{
public:
    int X;
    int Y;

    std::shared_ptr<IInnerData> sData;
    std::unique_ptr<IInnerData> uData;
    IInnerData* pData;

    B()
    {
        pData = nullptr;
        X = 0;
        Y = 0;
        //printf("B::B()\n");
    }

    ~B()
    {
        //printf("B::~B()\n");
    }
};

CRITICAL_SECTION cs;

unsigned int WINAPI Thread001(LPVOID param)
{
    LARGE_INTEGER Start, End;

    QueryPerformanceCounter(&Start);
    for (int i = 0; i < SAME_COUNT; i++)
    {
        spData* ptr = new spData();

        std::shared_ptr<B> b = std::make_unique<B>();
        b->pData = ptr;
        delete ptr;
    }
    QueryPerformanceCounter(&End);

    double New = (double)(End.QuadPart - Start.QuadPart) / (double)(Freq.QuadPart);

    EnterCriticalSection(&cs);
    printf("new : %fs \n", New);
    LeaveCriticalSection(&cs);

    return 0;
}

unsigned int WINAPI Thread002(LPVOID param)
{
    LARGE_INTEGER Start, End;

    QueryPerformanceCounter(&Start);
    for (int i = 0; i < SAME_COUNT; i++)
    {
        std::shared_ptr<spData> ptr = std::make_shared<spData>();

        std::shared_ptr<B> b = std::make_unique<B>();
        b->sData = ptr;
    }
    QueryPerformanceCounter(&End);

    double shared = (double)(End.QuadPart - Start.QuadPart) / (double)(Freq.QuadPart);

    EnterCriticalSection(&cs);
    printf("shared : %fs \n", shared);
    LeaveCriticalSection(&cs);

    return 0;
}

unsigned int WINAPI Thread003(LPVOID param)
{
    LARGE_INTEGER Start, End;

    QueryPerformanceCounter(&Start);
    for (int i = 0; i < SAME_COUNT; i++)
    {
        std::unique_ptr<spData> ptr = std::make_unique<spData>();

        std::shared_ptr<B> b = std::make_unique<B>();
        b->uData = std::move(ptr);
    }
    QueryPerformanceCounter(&End);

    double unique = (double)(End.QuadPart - Start.QuadPart) / (double)(Freq.QuadPart);

    EnterCriticalSection(&cs);
    printf("unique : %fs \n", unique);
    LeaveCriticalSection(&cs);

    return 0;
}

int main()
{
    timeBeginPeriod(1);

    InitializeCriticalSection(&cs);

    QueryPerformanceFrequency(&Freq);
    HANDLE handles[3];

    handles[2] = (HANDLE)_beginthreadex(nullptr, 0, Thread003, nullptr, 0, nullptr);
    handles[1] = (HANDLE)_beginthreadex(nullptr, 0, Thread002, nullptr, 0, nullptr);
    handles[0] = (HANDLE)_beginthreadex(nullptr, 0, Thread001, nullptr, 0, nullptr);
   
    WaitForMultipleObjects(3, handles, true, INFINITE);

    DeleteCriticalSection(&cs);
}
