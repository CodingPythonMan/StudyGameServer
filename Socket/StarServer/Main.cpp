#include "StarServer.h"

int main()
{
	ConsoleInit();

	if (SetServerSocket())
		return 0;

	if (ListenSocket())
		return 0;

	if (SelectLoop())
		return 0;

	if (EndSocket())
		return 0;
}