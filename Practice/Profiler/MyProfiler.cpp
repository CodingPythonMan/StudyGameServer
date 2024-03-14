#include "MyProfiler.h"

Profiler::Profiler()
{
	setlocale(LC_ALL, "");
	QueryPerformanceFrequency(&Freq);

	TLSIndex = TlsAlloc();
	if (TLSIndex == TLS_OUT_OF_INDEXES)
	{
		printf("TLS Alloc Error!\n");
	}
}

Profiler::~Profiler()
{
}

void Profiler::ProfileBegin(const WCHAR* szName)
{
	ProfileInfo* ProfileInfos = (ProfileInfo*)TlsGetValue(TLSIndex);
	if (ProfileInfos == nullptr)
	{
		ProfileInfos = new ProfileInfo[MAX_PROFILE];
		TlsSetValue(TLSIndex, (LPVOID)ProfileInfos);

		for (int i = 0; i < MAX_PROFILE; i++)
		{
			for (int j = 0; j < PROFILE_MIN_COUNT; j++)
				ProfileInfos[i]._min[j] = LLONG_MAX;
		}
	}

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

void Profiler::ProfileEnd(const WCHAR* szName)
{
	ProfileInfo* ProfileInfos = (ProfileInfo*)TlsGetValue(TLSIndex);

	for (int i = 0; i < MAX_PROFILE; i++)
	{
		if (ProfileInfos[i]._call > 0 && wcscmp(szName, ProfileInfos[i]._name) == 0)
		{
			ProfileCallFunc(i, szName);
			break;
		}
	}
}

void Profiler::ProfileDataOutText(const WCHAR* szFileName)
{
	ProfileInfo* ProfileInfos = (ProfileInfo*)TlsGetValue(TLSIndex);

	FILE* file = nullptr;
	_wfopen_s(&file, szFileName, L"a+");

	if (file == nullptr)
		return;

	const WCHAR* NameStr = L"Name";
	const WCHAR* AverageStr = L"Average";
	const WCHAR* MinStr = L"Min";
	const WCHAR* MaxStr = L"Max";
	const WCHAR* CallStr = L"Call";

	fwprintf(file, L"-----------------------------------------------------------------------------------\n");
	fwprintf(file, L"%20s  |%14s  |%14s |%14s |%14s |\n", NameStr, AverageStr, MinStr, MaxStr, CallStr);
	fwprintf(file, L"-----------------------------------------------------------------------------------\n");

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
			for (int j = 0; j < PROFILE_MIN_COUNT; j++)
			{
				if (ProfileInfos[i]._min[j] < LLONG_MAX)
				{
					total -= ProfileInfos[i]._min[j];
					minusCall++;
				}

				if (min > ProfileInfos[i]._min[j])
					min = ProfileInfos[i]._min[j];
			}

			for (int j = 0; j < PROFILE_MAX_COUNT; j++)
			{
				total -= ProfileInfos[i]._max[j];
				minusCall++;

				if (max < ProfileInfos[i]._max[j])
					max = ProfileInfos[i]._max[j];
			}

			average = (double)total / (double)Freq.QuadPart / (double)(call - minusCall) * 1000000;
			double minD = (double)min / (double)(Freq.QuadPart) * 1000000;
			double maxD = (double)max / (double)(Freq.QuadPart) * 1000000;

			fwprintf(file, L" %20s |   %10.4fμs |   %10.4fμs |   %10.4fμs |    %lld | \n", ProfileInfos[i]._name, average, minD, maxD, call);
		}
	}

	fwprintf(file, L"-------------------------------------------------------------------------------\n");

	fclose(file);
}

void Profiler::ProfileReset(void)
{
	ProfileInfo* ProfileInfos = (ProfileInfo*)TlsGetValue(TLSIndex);

	for (int i = 0; i < MAX_PROFILE; i++)
	{
		ProfileInfos[i]._call = 0;
		ProfileInfos[i]._totalTime = 0;
		for (int j = 0; j < PROFILE_MIN_COUNT; j++)
		{
			ProfileInfos[i]._min[j] = LLONG_MAX;
		}

		for (int j = 0; j < PROFILE_MAX_COUNT; j++)
		{
			ProfileInfos[i]._max[j] = 0;
		}
	}
}

void Profiler::ProfileCallFunc(int index, const WCHAR* szName)
{
	ProfileInfo* ProfileInfos = (ProfileInfo*)TlsGetValue(TLSIndex);

	LARGE_INTEGER End;
	QueryPerformanceCounter(&End);

	__int64 delta = End.QuadPart - ProfileInfos[index]._startTime.QuadPart;

	// Min 비교
	for (int i = 0; i < PROFILE_MIN_COUNT; i++)
	{
		if (ProfileInfos[index]._min[i] > delta)
		{
			ProfileInfos[index]._min[i] = delta;
			break;
		}
	}

	// Max 비교
	for (int i = 0; i < PROFILE_MAX_COUNT; i++)
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
