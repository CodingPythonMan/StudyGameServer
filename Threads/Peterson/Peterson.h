#include <windows.h>
#include <process.h>

unsigned int WINAPI Peterson001(LPVOID lpParam);
unsigned int WINAPI Peterson002(LPVOID lpParam);

extern int Items;