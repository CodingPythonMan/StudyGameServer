#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

Parser* Parser::GetInstance()
{
	static Parser parser;
	return &parser;
}

void Parser::LoadFile(const char* fileName)
{

}

void Parser::GetValueInt(const char* valueName, int* column)
{

}