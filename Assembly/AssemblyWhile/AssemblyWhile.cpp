// AssemblyWhile.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#define PRINT(T)    \
do{                   \
    printf(T);      \
}while(0)

int main()
{
    int x = 0;

	if (x == 0)
        PRINT("ddd");
    else
		PRINT("ddd");
    
    while (1)
    {
        x++;
    }

    x = 0xff;

    for (;;)
    {
        x++;
    }
}
