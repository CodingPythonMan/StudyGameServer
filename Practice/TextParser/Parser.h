#pragma once

class Parser
{
private:
	Parser();
	~Parser();

	char* buffer;

public:
	static Parser* GetInstance();

	void LoadFile(const char* fileName);
	void GetValueInt(const char* valueName, int* column);
	void GetNextWord(char* chrBuffer, int length);

	int Version;
	int ServerID;
};