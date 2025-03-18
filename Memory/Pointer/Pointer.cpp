#include <iostream>
#include <unordered_map>

int UniqueID = 0;

class FSPC
{
public:
    int X;
    int Y;

    FSPC()
    {
        X = UniqueID++;
        Y = UniqueID++;
    }

    void Print()
    {
        printf("Print X : %d, Y : %d\n", X, Y);
    }
};

void TestMap()
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

int main()
{
    std::vector<FSPC*> pcList;

    FSPC* newMember1 = new FSPC;
    FSPC* newMember2 = new FSPC;

    pcList.push_back(newMember1);
    pcList.push_back(newMember2);

    for (int i = 0; i < pcList.size(); i++)
    {
        if (pcList[i]->X == 0)
        {
            std::swap(pcList[0], pcList[i]);
            break;
        }
    }

    for (auto user : pcList)
    {
        user->Print();
    }
}