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
		printf("File2 Read Error !!! \n");
		return;
	}

	if (BitmapFileHeader.bfType != 0x4D42)
	{
		printf("File2 Header Error!!! \n");
		return;
	}

	ret = (int)fread_s(&BitmapInfoHeader, (int)sizeof(BitmapInfoHeader), 1, (int)sizeof(BitmapInfoHeader), file1);
	if (ret != sizeof(BitmapInfoHeader))
	{
		printf("File1 Read Error !!! \n");
		return;
	}

	ret = (int)fread_s(&BitmapInfoHeader, (int)sizeof(BitmapInfoHeader), 1, (int)sizeof(BitmapInfoHeader), file2);
	if (ret != sizeof(BitmapInfoHeader))
	{
		printf("File2 Read Error !!! \n");
		return;
	}

	// 픽셀 데이터 읽기
	int imageSize = (BitmapInfoHeader.biWidth * BitmapInfoHeader.biHeight) * 4;
	char* image1 = (char*)malloc(imageSize);
	if (image1 == nullptr)
		return;
	ret = (int)fread_s(image1, imageSize, 1, imageSize, file1);

	char* image2 = (char*)malloc(imageSize);
	if (image2 == nullptr)
		return;
	ret = (int)fread_s(image2, imageSize, 1, imageSize, file2);

	int fileResultSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + imageSize;
	char* imageResult = (char*)malloc(fileResultSize);

	if (imageResult == nullptr)
		return;

	memcpy(imageResult, &BitmapFileHeader, sizeof(BitmapFileHeader));
	memcpy(imageResult + sizeof(BitmapFileHeader), &BitmapInfoHeader, sizeof(BitmapInfoHeader));
	memcpy(imageResult + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader), image2, imageSize);

	// 픽셀 데이터 조절하기
	fwrite(imageResult, 1, fileResultSize, fileResult);

	free(image1);
	free(image2);
	free(imageResult);

	fclose(file1);
	fclose(file2);
	fclose(fileResult);
}

int main()
{
	AlphaBlending("sample.bmp", "sample2.bmp", "Result.bmp");
}