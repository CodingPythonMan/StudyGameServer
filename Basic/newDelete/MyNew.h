#pragma once
static class MyNew
{
	void* operator new(size_t size, char* File, int Line);
};