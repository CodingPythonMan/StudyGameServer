#include <iostream>
#include <process.h>
using namespace std;

int gData = 0;
int gConnect = 0;
bool gShutdown = false;

void AcceptThread()
{
	gConnect++;
}

void DisconnectThread()
{
	gConnect--;
}

void UpdateThread()
{

}

int main()
{
	_beginthreadex();
}