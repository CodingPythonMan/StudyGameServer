#include <iostream>
#include <memory>

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
};

class B
{
public:
    int X;
    int Y;

    std::shared_ptr<IInnerData> pData;

    B() 
    {
        pData = nullptr;
        X = 0;
        Y = 0;
        printf("B::B()\n"); 
    }

    ~B() 
    {
        printf("B::~B()\n"); 
    }
};

class C
{

};

class D : public C
{
public:
    std::shared_ptr<C> c;
};

int main()
{
    
    for (int i = 0; i < 1000; i++)
    {
        std::shared_ptr<spData> data = std::make_shared<spData>();

        std::shared_ptr<B> b = std::make_shared<B>();
        b->pData = data;
        b->pData = nullptr;
    }
}