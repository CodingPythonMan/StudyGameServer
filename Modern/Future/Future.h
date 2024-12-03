#pragma once
#include <future>
#include <iostream>
#include <thread>
#include <windows.h>
#include <tlhelp32.h>
#include <string>

class Future 
{
public:
	void Start();
	int GetThreadCount();
	bool ConvertTime(std::future<bool>& cursor);
};