#include "stdafx.h"

void Proc_0(){}
void Proc_1(){}
void Proc_2(){}
void Proc_3(){}
void Proc_4(){}
void Proc_5(){}

int main()
{
	int MessageType = 0;
	void (*ProcTable[6])(void);
	ProcTable[0] = Proc_0;
	ProcTable[1] = Proc_1;
	ProcTable[2] = Proc_2;
	ProcTable[3] = Proc_3;
	ProcTable[4] = Proc_4;
	ProcTable[5] = Proc_5;

	ProcTable[MessageType]();
}