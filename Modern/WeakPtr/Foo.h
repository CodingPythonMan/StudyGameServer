#pragma once
#include <iostream>
#include <memory>

struct Foo
{
    int id{ 0 };
    Foo(int i = 0) : id{ i } { std::cout << "Foo::Foo(" << i << ")\n"; }
    ~Foo() { std::cout << "Foo::~Foo(), id=" << id << '\n'; }

    void Start();
};
