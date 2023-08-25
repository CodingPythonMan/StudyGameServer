#include <stdio.h>
#include <malloc.h>
#include "StageRead.h"

char* sr_Stage_Read(const char* stage)
{
	FILE* file = nullptr;
	fopen_s(&file, stage, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)malloc(size);

	fread(buffer, 1, size, file);
	fclose(file);

	return buffer;
}