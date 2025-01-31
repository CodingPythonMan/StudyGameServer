#include <iostream>
#include <unordered_map>

class FSPC
{
public:
    int X;
    int Y;

    void Print()
    {
        printf("Print X : d, Y : d\n");
    }
};

int main()
{
    std::unordered_map<FSPC*, int> userMap;

    FSPC* pc = new FSPC;

    userMap.emplace(pc, 15);
    delete pc;

    int value = userMap[pc];
    printf("User Value : %d\n", value);

    for (auto user : userMap)
    {
        user.first->Print();
    }
}