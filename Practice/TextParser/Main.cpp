#include <iostream>
#include "Parser.h"

int main()
{
	Parser* parser = Parser::GetInstance();

	parser->LoadFile("GameConfig.ini");
	parser->GetValueInt("Version", &parser->Version);
	parser->GetValueInt("ServerID", &parser->ServerID);
	parser->GetValueStr("Path", &parser->Path);
	parser->GetValueStr("FileName", &parser->FileName);
}