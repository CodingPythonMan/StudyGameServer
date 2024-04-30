#include <iostream>
#include <windows.h>
#include <vector>

struct Hey {
	int a;
	int b;
	int c;
};

std::vector<Hey> heyList;

void NotifyHey()
{
	Hey hey;
	hey.a = 3;
	hey.b = 2;
	hey.c = 1;

	heyList.push_back(hey);
}

void Test()
{
	NotifyHey();

	printf("HeyList Size : %d\n", (int)heyList.size());
	printf("HeyList[0].a = %d\n", heyList[0].a);
	printf("HeyList[0].b = %d\n", heyList[0].b);
	printf("HeyList[0].c = %d\n", heyList[0].c);
}

void GetSystemCacheLine()
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = nullptr;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = nullptr;
	DWORD returnLength = 0;

	DWORD rc = GetLogicalProcessorInformation(buffer, &returnLength);

	printf("rc °ª : %d\n", returnLength);
}

int main()
{
	Test();
}
