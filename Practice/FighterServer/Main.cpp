#include "FighterServer.h"

int main()
{
	FighterServer* fighterServer = new FighterServer;

	do {
		if (true == fighterServer->Init())
			break;

		if (true == fighterServer->SelectLoop())
			break;

	} while (false);

	fighterServer->Close();
}