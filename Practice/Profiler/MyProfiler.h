#pragma once
#include <Windows.h>
#include <profileapi.h>
#include <cstdio>
#include <locale.h>

/*
#ifdef PROFILE
	#define PRO_BEGIN()	ProfileBegin()
	#define PRO_END()	ProfileEnd()
#endif
	#define PRO_BEGIN()
	#define PRO_END()*/

#define MIN_COUNT 2
#define MAX_COUNT 2

struct ProfileInfo
{
	__int64			_call;				// ���� ȣ�� Ƚ��.
	WCHAR			_name[64];			// �������� ���� �̸�.

	LARGE_INTEGER	_startTime;			// �������� ���� ���� �ð�.

	__int64			_totalTime;			// ��ü ���ð� ī���� Time.	(��½� ȣ��ȸ���� ������ ��� ����)
	__int64			_min[MIN_COUNT];			// �ּ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ּ� [1] ���� �ּ� [2])
	__int64			_max[MAX_COUNT];			// �ִ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ִ� [1] ���� �ִ� [2])
};

/////////////////////////////////////////////////////////////////////////////
// Profile Init
/////////////////////////////////////////////////////////////////////////////
void ProfileInit();

/////////////////////////////////////////////////////////////////////////////
// �ϳ��� �Լ� Profiling ����
/////////////////////////////////////////////////////////////////////////////
void ProfileBegin(const WCHAR* szName);

/////////////////////////////////////////////////////////////////////////////
// �ϳ��� �Լ� Profiling ��
/////////////////////////////////////////////////////////////////////////////
void ProfileEnd(const WCHAR* szName);

/////////////////////////////////////////////////////////////////////////////
// Profiling �� ����Ÿ�� Text ���Ϸ� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
void ProfileDataOutText(const WCHAR* szFileName);

/////////////////////////////////////////////////////////////////////////////
// �������ϸ� �� �����͸� ��� �ʱ�ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////
void ProfileReset(void);

/////////////////////////////////////////////////////////////////////////////
// ���� �Լ�
/////////////////////////////////////////////////////////////////////////////
void ProfileCallFunc(int index, const WCHAR* szName);