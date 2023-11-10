#pragma once

#define WAY 8
#define CACHE_INDEX 64

// 64bit 실행환경만 고려한다.
struct TagArray {
	unsigned __int64 tags[WAY];
	int roundRobin;
};

extern TagArray tagArrays[64];

void CacheSimulate(void* pointer);

void PrintCacheInfo(unsigned char cacheIndex, unsigned __int64 tag, unsigned __int64 address);