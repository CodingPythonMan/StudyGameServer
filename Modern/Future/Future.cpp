#include "Future.h"

void Future::Start()
{
    std::cout << "Initial thread count: " << GetThreadCount() << std::endl;

    {
        std::future<bool> cursor;

        if (cursor.valid())
        {
            cursor.get();
        }

        // ConvertTime 호출 전 스레드 수
        std::cout << "Before ConvertTime call, thread count: " << GetThreadCount() << std::endl;

        ConvertTime(cursor);

        // ConvertTime 호출 후 스레드 수
        std::cout << "After ConvertTime call, thread count: " << GetThreadCount() << std::endl;

        // ConvertTime을 호출하여 결과를 cursor에 저장
        cursor.get();
    }

    for (int i = 0; i < 100000; i++)
    {
        // ConvertTime 호출 후 스레드 수
        std::cout << "Future 가 삭제된 후 " << 2 * i << "초 후, thread count: " << GetThreadCount() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int Future::GetThreadCount()
{
    DWORD processID = GetCurrentProcessId();
    int threadCount = 0;

    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnapshot != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te;
        te.dwSize = sizeof(te);

        if (Thread32First(hThreadSnapshot, &te)) 
        {
            do 
            {
                if (te.th32OwnerProcessID == processID) 
                {
                    ++threadCount;
                }
            } 
            while (Thread32Next(hThreadSnapshot, &te));
        }
        CloseHandle(hThreadSnapshot);
    }

    return threadCount;
}

bool Future::ConvertTime(std::future<bool>& cursor)
{
    auto asyncTask = []() -> bool 
    {
        std::string s1("21:32:10");
        std::string s2("19:32:10");

        if (s1.empty() && s2.empty())
        {
            return false;
        }

        std::cout << s1 << "\n";
        std::cout << s2 << "\n";

        return true;
    };

    cursor = std::async(std::launch::async, asyncTask);

    return true;
}