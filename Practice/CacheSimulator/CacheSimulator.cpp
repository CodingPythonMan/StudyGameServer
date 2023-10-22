#include <iostream>
#include "CacheSimulator.h"

// 64 bit
// 캐시라인이 들어있는 장소를 인덱스로 지정한다.

// Offset 6비트는 캐시라인 오프셋인 64바이트에서 각 바이트 오프셋을 표현할 수 있다.
// 1바이트당 하나씩 주소값을 할당 받는다.

// 52bit-------------- Tag | 6 bit--------------- Cache Index | 6bit----------------- Offset
// Data Cache 용량에 맞춰 Way 수가 결정될 것이다. 
// 다만, 미리 계산할 수 있는 경우 WAY 를 넣고 한다.

TagArray tagArrays[64];

void CashSimulate(void* pointer)
{
	unsigned char cacheIndex;
	unsigned __int64 tag;

	unsigned __int64 address = (unsigned __int64)pointer;

	// 6bit
	cacheIndex = (unsigned char)(address >> 6);
	cacheIndex <<= 2;
	cacheIndex >>= 2;
	// 52bit
	tag = (unsigned __int64)(address >> 12);

	//PrintCacheInfo(cacheIndex, tag, address);

	bool cacheHit = false;
	for (int i = 0; i < WAY; i++)
	{
		// Cache Hit!
		if (tag == tagArrays[cacheIndex].tags[i])
		{
			cacheHit = true;
			printf("Cache Hit!\n");
			break;
		}
	}

	// 원래 라운드 로빈으로 태그 교체하던 걸 LRU로 변경이 필요하긴 하다.
	if (cacheHit == false)
	{
		printf("Cache Miss!\n");
		int roundRobin = tagArrays[cacheIndex].roundRobin;
		tagArrays[cacheIndex].tags[roundRobin] = tag;
		if (roundRobin >= WAY)
		{
			tagArrays[cacheIndex].roundRobin = 0;
		}
		else
		{
			tagArrays[cacheIndex].roundRobin++;
		}
	}
}

void PrintCacheInfo(unsigned char cacheIndex, unsigned __int64 tag, unsigned __int64 address)
{
	printf("Cache Index : %x \n", cacheIndex);
	printf("Tag : %llx \n", tag);
	printf("Logical Address : %llx \n", address);
}