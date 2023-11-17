#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

Parser::Parser() : Version{ 0 }, ServerID{ 0 }, buffer{ nullptr}
{
}

Parser::~Parser()
{
	free(buffer);
}

Parser* Parser::GetInstance()
{
	static Parser parser;
	return &parser;
}

void Parser::LoadFile(const char* fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "r+");

	if (file == nullptr)
		return;

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(length + 1);
	if (buffer == nullptr)
		return;

	fread_s(buffer, length + 1, 0, length, file);

	fclose(file);
}

void Parser::GetValueInt(const char* valueName, int* column)
{
	


}

void Parser::GetNextWord(char* chrBuffer, int length)
{
	chrBuffer = buffer;

	while (1)
	{
		// 0x08 백스페이스
		// 0x09 탭
		// 0x0a 라인 피드
		// 0x0d 캐리지 리턴
		if (*chrBuffer == ',' || *chrBuffer == '.' || *chrBuffer == '"' || *chrBuffer == 0x20
			|| *chrBuffer == 0x08 || *chrBuffer == 0x09 || *chrBuffer == 0x0a || *chrBuffer == 0x0d)
		{
			break;
		}

		chrBuffer++;
	}
}