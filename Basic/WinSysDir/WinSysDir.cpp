#include <stdio.h>
#include <windows.h>

#define DIR_LEN MAX_PATH+1

enum class COMMAND{
    DIV = 1,
    MUL,
    ADD,
    MIN,
    ELSE,
    EXIT
};

COMMAND ShowMenu();
void Divide(double, double);
void Multiple(double, double);
void Add(double, double);
void Min(double, double);

int wmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = L"calc.exe";
    SetCurrentDirectory(L"C:\\WINMDOWS\\system32");

    COMMAND sel;
    double num1, num2;
    while (true)
    {
        sel = ShowMenu();
        if (sel == COMMAND::EXIT)
        {
            return 0;
        }

        if (sel != COMMAND::ELSE)
        {
            fputws(L"Input Num1 Num2: ", stdout);
            wscanf_s(L"%lf %lf", &num1, &num2);
        }

        switch (sel)
        {
        case COMMAND::DIV:
            Divide(num1, num2);
            break;
        case COMMAND::MUL:
            Multiple(num1, num2);
            break;
        case COMMAND::ADD:
            Add(num1, num2);
            break;
        case COMMAND::MIN:
            Min(num1, num2);
            break;
        case COMMAND::ELSE:
            ZeroMemory(&pi, sizeof(pi));
            CreateProcess(NULL, command, NULL, NULL,
                TRUE, 0, NULL, NULL, &si, &pi);
            break;
        }
    }

    return 0;
}

COMMAND ShowMenu()
{
    COMMAND sel;
    int selInt;

    fputws(L"-----Menu----- \n", stdout);
    fputws(L"num1: Divide \n", stdout);
    fputws(L"num2: Multiple \n", stdout);
    fputws(L"num3: Add \n", stdout);
    fputws(L"num4: Minus \n", stdout);
    fputws(L"num5: Any other operations. \n", stdout);
    fputws(L"num6: Exit \n", stdout);
    fputws(L"SELECTION >>", stdout);
    scanf_s("%d", &selInt);

    sel = static_cast<COMMAND>(selInt);

    return sel;
}
void Divide(double a, double b)
{
    wprintf(L"%f/%f=%f \n\n", a, b, a / b);
}
void Multiple(double a, double b)
{
    wprintf(L"%fx%f=%f \n\n", a, b, a * b);
}
void Add(double a, double b)
{
	wprintf(L"%f+%f=%f \n\n", a, b, a + b);
}
void Min(double a, double b)
{
	wprintf(L"%f-%f=%f \n\n", a, b, a - b);
}