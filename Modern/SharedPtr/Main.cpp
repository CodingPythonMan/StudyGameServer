#include <chrono>
//#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "Item.h"

using namespace std::chrono_literals;

struct Base
{
    Base() { std::cout << "기본::기본()\n"; }

    // non-virtual 소멸자도 여기선 괜찮다.
    ~Base() { std::cout << "기본::~기본()\n"; }
};

struct Derived : public Base
{
    Derived() { std::cout << "하위::하위()\n"; }

    ~Derived() { std::cout << "하위::~하위()\n"; }
};

void print(auto rem, std::shared_ptr<Base> const& sp)
{
    std::cout << rem << "\n\tget() = " << sp.get()
        << ", use_count() = " << sp.use_count() << '\n';
}

void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(987ms);
    std::shared_ptr<Base> lp = p; // 스레드 세이프 해져서 그냥 실행한다.

    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        print("스레드 안 로컬 포인터:", lp);
    }
}

void CraftItem(Item* item)
{
    //std::this_thread::sleep_for(10000s);
}

int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();

    print("Shared Derived 클래스 포인터를 만들었다. (Base 포인터에 담았다.)", p);

    std::thread t1{ thr, p }, t2{ thr, p }, t3{ thr, p };
    p.reset(); // main 의 소유권을 포기한다.

    print("3개의 스레드 간 Shared 된 소유권을 main 은 포기한다:", p);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "모든 스레드가 끝나고, 마지막에 소멸자가 호출된다.\n\n";
    
    std::cout << "그럼, Shared_ptr 자체는 인자로 넘겨준 순간 reference Count 가 증가하는 걸까?\n";

    std::shared_ptr<Derived> myDerived = std::make_shared<Derived>();

    thr(myDerived);
    myDerived.reset();
    std::cout << "여기서 참조 카운트는 3이 찍히겠군.\n\n";

    std::cout << "unordered_map 에 value 로서 있는 걸 넘겨준다는 개념은 어떨까?\n";
    std::unordered_map<int, Item::SharedPtr> ItemList;

    Item::SharedPtr addItem = Item::New();
    Item::SharedPtr addItem2 = Item::New();

    ItemList.insert({ 1, addItem });
    //ItemList.insert({ 2, addItem2 });

    //std::thread t4{ CraftItem, ItemList[1].get() };

    CraftItem(ItemList[1].get());

    //addItem.reset();

    std::cout << addItem << "\n\tget() = " << addItem.get()
        << ", use_count() = " << addItem.use_count() << '\n';
}