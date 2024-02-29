#include "MyProfiler.h"

LARGE_INTEGER End;
LARGE_INTEGER Freq;

#define MAX_PROFILE 20

ProfileInfo ProfileInfos[MAX_PROFILE];

void ProfileInit()
{
	setlocale(LC_ALL, "");
	QueryPerformanceFrequency(&Freq);

	for (int i = 0; i < MAX_PROFILE; i++)
	{
		for (int j = 0; j < MIN_COUNT; j++)
			ProfileInfos[i]._min[j] = LLONG_MAX;
	}
}

void ProfileBegin(const WCHAR* szName)
{
	bool exist = false;
	for (int i = 0; i < MAX_PROFILE; i++)
	{
		if (ProfileInfos[i]._call > 0 && wcscmp(szName, ProfileInfos[i]._name) == 0)
		{
			ProfileInfos[i]._call++;
			wcscpy_s(ProfileInfos[i]._name, szName);
			QueryPerformanceCounter(&ProfileInfos[i]._startTime);
			exist = true;
			break;
		}
	}

	if (exist == false)
	{
		for (int i = 0; i < MAX_PROFILE; i++)
		{
			if (ProfileInfos[i]._call < 1)
			{
				ProfileInfos[i]._call++;
				wcscpy_s(ProfileInfos[i]._name, szName);
				QueryPerformanceCounter(&ProfileInfos[i]._startTime);
				break;
			}
		}
	}
}

void ProfileEnd(const WCHAR* szName)
{
	QueryPerformanceCounter(&End);
	
	for (int i = 0; i < MAX_PROFILE; i++)
	{ 
		if (ProfileInfos[i]._call > 0 && wcscmp(szName, ProfileInfos[i]._name) == 0)
		{
			ProfileCallFunc(i, szName);
			break;
		}
	}
}

void ProfileCallFunc(int index, const WCHAR* szName)
{
	__int64 delta = End.QuadPart - ProfileInfos[index]._startTime.QuadPart;

	// Min 비교
	for (int i = 0; i < MIN_COUNT; i++)
	{
		if (ProfileInfos[index]._min[i] > delta)
		{
			ProfileInfos[index]._min[i] = delta;
			break;
		}
	}

	// Max 비교
	for (int i = 0; i < MAX_COUNT; i++)
	{
		if (ProfileInfos[index]._max[i] < delta)
		{
			ProfileInfos[index]._max[i] = delta;
			break;
		}
	}

	// Total Time 계산
	ProfileInfos[index]._totalTime += delta;
}

void ProfileDataOutText(const WCHAR* szFileName)
{

	FILE* file = nullptr;
	_wfopen_s(&file, szFileName, L"w");

	if (file == nullptr)
		return;

	fwprintf(file, L"-------------------------------------------------------------------------------\n\n");
	fwprintf(file, L"           Name  |     Average  |        Min   |        Max   |      Call |\n");
	fwprintf(file, L"-------------------------------------------------------------------------------\n");

	for (int i = 0; i < MAX_PROFILE; i++)
	{
		__int64 call = ProfileInfos[i]._call;

		if (call > 0)
		{
			double average;
			__int64 total = ProfileInfos[i]._totalTime;

			__int64 max = 0;
			__int64 min = ProfileInfos[i]._min[0];

			int minusCall = 0;
			for (int j = 0; j < MIN_COUNT; j++)
			{
				if (ProfileInfos[i]._min[j] < LLONG_MAX)
				{
					total -= ProfileInfos[i]._min[j];
					minusCall++;
				}

				if (min > ProfileInfos[i]._min[j])
					min = ProfileInfos[i]._min[j];
			}

			for (int j = 0; j < MAX_COUNT; j++)
			{
				total -= ProfileInfos[i]._max[j];
				minusCall++;

				if (max < ProfileInfos[i]._max[j])
					max = ProfileInfos[i]._max[j];
			}

			average = (double)total / (double)Freq.QuadPart / (double)(call - minusCall) * 1000;
			double minD = (double)min / (double)(Freq.QuadPart) * 1000;
			double maxD = (double)max / (double)(Freq.QuadPart) * 1000;

			fwprintf(file, L" %12s |   %fms |   %fms |   %fms |    %lld \n", ProfileInfos[i]._name, average, minD, maxD, call);
		}
	}

	fwprintf(file, L"\n\n-------------------------------------------------------------------------------\n");

	fclose(file);
}

void ProfileReset(void)
{
	for (int i = 0; i < MAX_PROFILE; i++)
	{
		ProfileInfos[i]._call = 0;
		ProfileInfos[i]._totalTime = 0;
		for (int j = 0; j < MIN_COUNT; j++)
		{
			ProfileInfos[i]._min[j] = LLONG_MAX;
		}

		for (int j = 0; j < MAX_COUNT; j++)
		{
			ProfileInfos[i]._max[j] = 0;
		}
	}
}