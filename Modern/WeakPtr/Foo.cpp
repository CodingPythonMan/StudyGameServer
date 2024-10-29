#include "Foo.h"

void Foo::Start()
{
    std::shared_ptr<Foo> sh4(new Foo{ 11 }, [](Foo* p) {
        std::cout << "Call delete from function object. Foo::id=" << p->id << '\n';
        delete p;
        });
}
