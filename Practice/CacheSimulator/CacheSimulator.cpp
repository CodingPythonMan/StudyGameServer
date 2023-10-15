#include <iostream>

struct Way {
	__int64 cacheLine;
};

struct Cache {
	Way way[8];
};

Cache cache[64];

// 64 bit
// 캐시라인이 들어있는 장소를 인덱스로 지정한다.
// 메인 메모리는 우선 균일한 page(4kb = 2^12)로 나누어져 있다.
// Offset 이 6비트인 이유? => 내 pc 를 보니 2코어에 L1 이 128kb 이다.
// 캐시라인 Offset 이라는게 무슨 뜻일까. => 하나의 주소가 1개 바이트를 가리킨다.
// Indirect 방식을 의미하는 건가? 0000 0000 => 그럼 메모리 시작 위치는 어디에 표시하고 있는가?
// 이게 정해지는게 중요하다. 
// 캐시 Index. 캐시 인덱스는 뭘 나타내는가? => 몇 번 캐시블록을 가리키는지 표시한다.
// 이게 이제 CPU 에서 참조하고 싶어하는 캐시값이란 소린데.

// Offset 6비트는 캐시라인 오프셋인 64바이트에서 각 바이트 오프셋을 표현할 수 있다.
// 1바이트당 하나씩 주소값을 할당 받는다.

// 52bit-------------- Tag | 6 bit--------------- Index | 6bit----------------- Offset

void CashSimulate(int* pointer)
{
	void* address = (void*)pointer;

	printf("0x%p \n", address);
}