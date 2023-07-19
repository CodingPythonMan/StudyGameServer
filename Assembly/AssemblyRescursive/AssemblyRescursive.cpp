#include <iostream>

void Test(int a)
{
    char b[10];
    b[0] = 0;
    a = a + 1;
    if (a > 1000000)
        return;

    printf("%d \n", a);
    Test(a);
}

int main()
{
    Test(0);


}