#include <iostream>

// 1. strlen - 문자열 길이 계산
int my_strlen(char* str)
{
    int length = 0;
    
    while (*str != '\0')
    {
        str++;
        length++;
    }

    return length;
}

// 2. strcpy - 문자열 복사
char* my_strcpy(char *strDest, const char *strSrc)
{
    while (*strSrc != '\0')
    {
        *strDest = *strSrc;
        strDest++;
        strSrc++;
    }

    return strDest;
}

// 3. strcmp - 문자열 비교
// 4. strcat - 문자열 붙이기
// 5. strchr - 문자열 내에서 특정 문자 검색 (포인터 리턴)
// 6. strstr - 문자열 내에서 특정 문자열 검색 (포인터 리턴)
// 7. strlwr - 문자열을 소문자로 변환

int main()
{
    char name[50] = "12312435";
    char name2[50];

    my_strcpy(name2, name);
    
    printf("%d\n", my_strlen(name2));
}