#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

WCHAR ERROR_CMD[]
= L"'%s'는 실행할 수 있는 프로그램이 아닙니다. \n";

WCHAR cmdString[STR_LEN];
WCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
WCHAR seps[] = L" ,\t\n";

int CmdReadTokenize(void);
int CmdProcessing(int);
WCHAR* StrLower(WCHAR*);

int StartEchoCommand(int argc, WCHAR* argv[])
{
	// 한글 입력을 가능케 하기 위해
	_wsetlocale(LC_ALL, L"Korean");

	// 매개변수 전달인자가 있는 경우 처리를 위한 것이다
	// Start 명령어를 위한 처리로 생각
	if (argc > 2)
	{
		for (int i = 1; i < argc; i++)
			wcscpy_s(cmdTokenList[i - 1], argv[i]);
		CmdProcessing(argc - 1);
	}

	DWORD isExit = NULL;
	while (1)
	{
		int tokenNum = CmdReadTokenize();

		if (tokenNum == 0) // Enter 입력 처리를 위한 부분
			continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit == TRUE)
		{
			fputws(L" 명령어 처리를 종료합니다. \n", stdout);
			break;
		}
	}

	return 0;
}

int CmdReadTokenize(void)
{
	WCHAR* token = nullptr;
	WCHAR* nextToken = nullptr;

	fputws(L"Best command Prompt>> ", stdout);
	_getws_s(cmdString);

	token = wcstok_s(cmdString, seps, &nextToken);

	int tokenNum = 0;

	while (token != nullptr)
	{
		wcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
		token = wcstok_s(nullptr, seps, &nextToken);
	}

	return tokenNum;
}

int CmdProcessing(int tokenNum)
{
	BOOL isRun;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	WCHAR cmdStringWithOptions[STR_LEN] = { 0, };
	WCHAR optString[STR_LEN] = { 0, };

	si.cb = sizeof(si);
	if (!wcscmp(cmdTokenList[0], L"exit"))
	{
		return TRUE;
	}
	else if (!wcscmp(cmdTokenList[0], L"start"))
	{
		// start echo test good 이라는 문자열이 입력되었다고 가정하자    
		if (tokenNum > 1)
		{
			// start 를 제외한 나머지 문자열을 재구성한다
			for (int i = 1; i < tokenNum; i++)
			{
				wsprintf(optString, L"%s %s", optString, cmdTokenList[i]);
				wsprintf(cmdStringWithOptions, L"%s %s", L"WinConsole003.exe", optString);
			}
		}
		else // start 입력하는 경우 처리
		{
			wprintf(cmdStringWithOptions, L"%s", L"WinConsole003.exe");
		}

		isRun = CreateProcessW(NULL, cmdStringWithOptions, NULL, NULL,
			TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if (!wcscmp(cmdTokenList[0], L"echo"))
	{
		// 입력된 문자열을 그대로 출력하는 echo 명령어
		for (int i = 0; i < tokenNum; i++)
			wsprintf(optString, L"%s %s", optString, cmdTokenList[i]);

		wprintf(L"echo message: %s \n", optString);
	}
	else
	{
		wcscpy_s(cmdStringWithOptions, cmdTokenList[0]);

		// Check Point 1 : 문자열 처리를 위한 for문
		for (int i = 1; i < tokenNum; i++)
			wsprintf(cmdStringWithOptions, L"%s %s", cmdStringWithOptions, cmdTokenList[i]);

		isRun = CreateProcessW(NULL, cmdStringWithOptions, NULL, NULL,
			TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == FALSE)
			wprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}

WCHAR* StrLower(WCHAR* Str)
{
	WCHAR* ret = Str;

	while (*Str)
	{
		if (_istupper(*Str))
		{
			*Str = towlower(*Str);
		}

		Str++;
	}

	return ret;
}