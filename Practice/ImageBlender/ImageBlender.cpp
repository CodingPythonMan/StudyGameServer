#include <iostream>
#include <Windows.h>

void AlphaBlending(const char* bmp1, const char* bmp2, const char* result)
{
	FILE* file1;
	FILE* file2;
	FILE* fileResult;

	fopen_s(&file1, bmp1, "rb");
	fopen_s(&file2, bmp2, "rb");
	fopen_s(&fileResult, result, "wb");

	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFOHEADER BitmapInfoHeader;

	if (file1 == nullptr || file2 == nullptr || fileResult == nullptr)
		return;

	int ret;
	ret = (int)fread_s(&BitmapFileHeader, (int)sizeof(BitmapFileHeader), 1, (int)sizeof(BitmapFileHeader), file1);
	if (ret != sizeof(BitmapFileHeader))
	{
		printf("File1 Read Error !!! \n");
		return;
	}

	if (BitmapFileHeader.bfType != 0x4D42)
	{
		printf("File1 Header Error!!! \n");
		return;
	}

	ret = (int)fread_s(&BitmapFileHeader, (int)sizeof(BitmapFileHeader), 1, (int)sizeof(BitmapFileHeader), file2);
	if (ret != sizeof(BitmapFileHeader))
	{
		printf("File1 Read Error !!! \n");
		return;
	}

	if (BitmapFileHeader.bfType != 0x4D42)
	{
		printf("File1 Header Error!!! \n");
		return;
	}

	int imageSize = BitmapInfoHeader.biWidth * BitmapInfoHeader.biHeight;


	fclose(file1);
	fclose(file2);
	fclose(fileResult);
}

int main()
{
	AlphaBlending("sample.bmp", "sample2.bmp", "Result.bmp");
}