#pragma once

class Parser
{
private:
	Parser();
	~Parser();

public:
	static Parser* GetInstance();

	void LoadFile(const char* fileName);
	void GetValueInt(const char* valueName, int* column);

	int Version;
	int ServerID;
};