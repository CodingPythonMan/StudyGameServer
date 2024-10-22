#include <iostream>
#include <future>

std::future<bool> cursor;

bool ConvertTime()
{
    auto asyncTask = []() -> bool {
        std::string s1("21:32:10");
        std::string s2("19:32:10");

        if (s1.empty() && s2.empty())
        {
            return false;
        }

        std::cout << s1 << "\n";
        std::cout << s2;

        return true;
        };

    cursor = std::async(std::launch::async, asyncTask);

    return true;
}

int main()
{
    if (cursor.valid())
    {
        cursor.get();
    }

    ConvertTime();

    // ConvertTime을 호출하여 결과를 cursor에 저장
    cursor.get();
}