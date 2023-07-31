#include <iostream>

int custom_rand()
{

}

int main()
{
    srand(100);

    int x = rand();
    printf("%d\n", x);
    x = rand();
    printf("%d\n", x);
    x = rand();
    printf("%d\n", x);
    x = rand();
    printf("%d\n", x);
}