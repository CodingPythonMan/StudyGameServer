#pragma once
#include <Windows.h>
#include <profileapi.h>
#include <cstdio>
#include <locale.h>

#define PROFILE_MIN_COUNT 2
#define PROFILE_MAX_COUNT 2

#define MAX_PROFILE 20

class Profiler {
public:
	static Profiler* GetInstance(void)
	{
		static Profiler profiler;
		return &profiler;
	}

private:
	Profiler();
	~Profiler();

public:
	/////////////////////////////////////////////////////////////////////////////
	// 하나의 함수 Profiling 시작
	/////////////////////////////////////////////////////////////////////////////
	void ProfileBegin(const WCHAR* szName);

	/////////////////////////////////////////////////////////////////////////////
	// 하나의 함수 Profiling 끝
	/////////////////////////////////////////////////////////////////////////////
	void ProfileEnd(const WCHAR* szName);

	/////////////////////////////////////////////////////////////////////////////
	// Profiling 된 데이타를 Text 파일로 출력한다.
	/////////////////////////////////////////////////////////////////////////////
	void ProfileDataOutText(const WCHAR* szFileName);

	/////////////////////////////////////////////////////////////////////////////
	// 프로파일링 된 데이터를 모두 초기화 한다.
	/////////////////////////////////////////////////////////////////////////////
	void ProfileReset(void);

	/////////////////////////////////////////////////////////////////////////////
	// 내부 함수
	/////////////////////////////////////////////////////////////////////////////
	void ProfileCallFunc(int index, const WCHAR* szName);

private:
	int TLSIndex;

	LARGE_INTEGER Freq;
};

struct ProfileInfo
{
	__int64			_call;						// 누적 호출 횟수.
	WCHAR			_name[64];					// 프로파일 샘플 이름.

	LARGE_INTEGER	_startTime;					// 프로파일 샘플 실행 시간.

	__int64			_totalTime;					// 전체 사용시간 카운터 Time.	(출력시 호출회수로 나누어 평균 구함)
	__int64			_min[PROFILE_MIN_COUNT];	// 최소 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최소 [1] 다음 최소 [2])
	__int64			_max[PROFILE_MAX_COUNT];	// 최대 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최대 [1] 다음 최대 [2])

	ProfileInfo()
	{
		_call = 0;
		_totalTime = 0;
	}
};