#include <iostream>
#include <windows.h>

int main()
{
	FILETIME ft = {};
	int64_t tick = 0;

	GetSystemTimeAsFileTime(&ft);

	tick = ((long long)ft.dwHighDateTime << 32) + (((long long)ft.dwLowDateTime));
	tick = tick / 10000;
	
	if (tick == 0ll)
		return 0;

	//time_t tRet = ((val * 10000ll) - 116444736000000000ll) / 10000000ll;
	// faster
	time_t tRet = (tick - 11644473600000ll) / 1000ll;

	printf("UTC : %lld\n", tRet);
}