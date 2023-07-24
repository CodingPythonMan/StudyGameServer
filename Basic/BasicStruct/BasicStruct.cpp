#include <iostream>

struct stDATA {
    int a;
    int b;
    int c;
};

int main()
{
    stDATA data;
    data.a = 0;
    data.b = 0;
    data.c = 0;

    int x = data.c + 1;
    
    stDATA* p = &data;
    p->a = 0;
    p->b = 0;
    p->c = 0;
}