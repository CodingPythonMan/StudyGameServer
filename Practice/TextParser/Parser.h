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
	bool GetValueInt(const char* valueName, int* column);
	bool GetNextWord(char** chrBuffer, int* length);
	bool SkipNoneCommand(char* chrBuffer);

	int Version;
	int ServerID;
};