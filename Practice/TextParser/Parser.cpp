#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser::Parser() : Version{ 0 }, ServerID{ 0 }, buffer{ nullptr }
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
	buffer[length] = '\0';

	fclose(file);
}

bool Parser::GetValueInt(const char* valueName, int* column)
{
	int length = 0;
	char word[256];
	char* chrBuffer = buffer;
	
	// Load 당시 { 를 찾고 시작한다.
	while (1)
	{
		if (*chrBuffer == '}')
			break;

		if (*chrBuffer == '\0')
			return;

		chrBuffer++;
	}

	while (GetNextWord(&chrBuffer, &length))
	{
		// Word 버퍼에 찾은 단어 저장
		memset(word, 0, 256);
		memcpy(word, chrBuffer, length);
		word[255] = '\0';

		// 인자로 입력받은 단어와 같은지 검사한다.
		if (0 == strcmp(valueName, word))
		{
			// 바로 뒤 = 을 찾는다.

		}
	}

	return false;
}

bool Parser::GetNextWord(char** chrBufferPtr, int* lengthPtr)
{
	char* chrBuffer = *chrBufferPtr;

	while (1)
	{
		if (SkipNoneCommand(chrBuffer))
		{
			return false;
		}
		
		if (*chrBuffer == ',' || *chrBuffer == '.' || *chrBuffer == '"' || *chrBuffer == 0x08 || *chrBuffer == '}')
		{
			break;
		}

		chrBuffer++;
	}

	if (*chrBuffer == '}')
		return false;
	
	return true;
}

bool Parser::SkipNoneCommand(char* chrBuffer)
{
	while (1)
	{
		// 주석처리 스킵


		// 스페이스, 탭, 엔터코드
		// 0x20 스페이스
		// 0x08 백스페이스
		// 0x09 탭
		// 0x0a 라인 피드
		// 0x0d 캐리지 리턴
		if (*chrBuffer == 0x20 || *chrBuffer == 0x09 || *chrBuffer == 0x0a || *chrBuffer == 0x0d)
		{
			chrBuffer++;
		}
		else
		{
			break;
		}
	}
	
	if(*chrBuffer == '\0' || *chrBuffer == '}')
		return true;

	return false;
}