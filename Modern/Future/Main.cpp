#include "Future.h"

int main()
{
    std::vector<std::future<bool>> futures;
    futures.resize(20);

    for (int a = 0; a < 5; a++)
    {
        for (int i = 0; i < 2; i++)
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

            futures[0] = std::async(std::launch::async, asyncTask);
        }
    }
    
}