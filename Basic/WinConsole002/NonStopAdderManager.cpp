#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// NonStopAdderManager.cpp

int NonStopAdderManage()
{
	STARTUPINFO si1 = { 0, };
	STARTUPINFO si2 = { 0, };

	PROCESS_INFORMATION pi1;
	PROCESS_INFORMATION pi2;

	HANDLE handles[2];

	DWORD return_val1;
	DWORD return_val2;

	WCHAR command1[] = L"WinConsole001.exe 1 5";
	WCHAR command2[] = L"WinConsole001.exe 6 10";

	DWORD sum = 0;

	si1.cb = sizeof(si1);
	si2.cb = sizeof(si2);

	CreateProcessW(nullptr, command1,
		nullptr, nullptr, TRUE, 0, nullptr, nullptr,
		&si1, &pi1);

	CreateProcessW(nullptr, command2,
		nullptr, nullptr, TRUE, 0, nullptr, nullptr,
		&si2, &pi2);

	CloseHandle(pi1.hThread);
	CloseHandle(pi2.hThread);

	// Wait �� �ؼ� �ڽ� ���μ����� ������ ����� ������ ��ٷ����Ѵ�.
	// Non-Signaled ���¿��� Signaled ���°� �� ������ ��ٸ��� �Լ��̴�.
	//WaitForSingleObject(pi1.hProcess, INFINITE);
	//WaitForSingleObject(pi2.hProcess, INFINITE);

	handles[0] = pi1.hProcess;
	handles[1] = pi2.hProcess;

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	GetExitCodeProcess(pi1.hProcess, &return_val1);
	GetExitCodeProcess(pi2.hProcess, &return_val2);

	if (return_val1 == -1 || return_val2 == -1)
		return -1; // �������� ����

	sum += return_val1;
	sum += return_val2;

	wprintf(L"total : %d \n", sum);

	CloseHandle(pi1.hProcess);
	CloseHandle(pi2.hProcess);

	return 0;
}