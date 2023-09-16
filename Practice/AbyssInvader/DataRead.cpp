#include <stdio.h>
#include <malloc.h>
#include <string.h>

char* d_Data_Read(const char* data)
{
	char* buffer = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, data, "r");

	if (file == nullptr) // warning C6387 : 파일 없음 예외 처리를 해 주어야 한다.
	{
		printf("There is No File. \n");
		return buffer;
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(size);

	if (buffer == nullptr)
	{
		printf("There is No Buffer. \n");
		return buffer;
	}

	fread(buffer, 1, size, file);
	fclose(file);

	return buffer;
}

void d_MovePattern_Set()
{
	const char* MovePatternInfo = "MovePatternInfo.data";

	d_Data_Read(MovePatternInfo);


}