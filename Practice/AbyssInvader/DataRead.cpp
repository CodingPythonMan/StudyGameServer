#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "DataRead.h"

char* d_Data_Read(const char* data)
{
	FILE* file = nullptr;
	fopen_s(&file, data, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)malloc(size);

	fread(buffer, 1, size, file);
	fclose(file);

	return buffer;
}

void d_MovePattern_Set(const char* data)
{
	char* buffer = d_Data_Read(_MonsterInfo);

	for (int i = 0; i < (int)strlen(buffer); i++)
	{
		if (buffer[i] == '\n')
		{
			defaultX = 31;
			defaultY += 1;
		}
	}
}

d_Monster_Set(_MonsterInfo);
d_Stage_Set(_StageInfo);