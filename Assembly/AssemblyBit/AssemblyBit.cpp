// AssemblyBit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void PrintBit(unsigned char a)
{
    printf("a : ");
    for (int i = 0; i < sizeof(a) * 8; i++)
    {
        printf("%d", (a & 1 << (7 - i)) >> (7 - i));
    }
}

void ChangeBit()
{
    unsigned short value = 0;
    unsigned short temp;

    int bitSize = sizeof(value) * 8;

    while (true)
    {
        int pos = 0;
        int input = 0;
        printf("비트위치 : ");
        scanf_s("%d", &pos);
        if (pos <= 0 || pos > bitSize)
        {
            printf("값이 잘 못 입력되었습니다.");
            continue;
        }
        
        printf("OFF/ON [0,1] : ");
        scanf_s("%d", &input);

        if (input != 0)
        {
            value = value | (1 << (pos-1));
        }
        else
        {
            temp = ~(1 << (pos-1));
            value = value & temp;
        }

        for (int i = 0; i < bitSize; i++)
        {
            temp = value & (1 << (bitSize - i - 1));
            
            printf("%d 번 Bit : ", bitSize - i);
            
            if (temp != 0)
                printf("ON");
            else
                printf("OFF");

            printf("\n");
        }
        printf("\n");
    }
}

void SharedBit()
{
    unsigned int a = 0;

    for (;;)
    {
		int pos = 0;
        printf("바이트 위치 : ");
		scanf_s("%d", &pos);

        if (pos < 1 || pos > 4)
        {
            printf("잘못된 값이 입력 되었습니다.");
            continue;
        }

        printf("0~255 : ");
        
    }
}

int main()
{
    //unsigned char a = 101;

    //PrintBit(a);
    ChangeBit();
}