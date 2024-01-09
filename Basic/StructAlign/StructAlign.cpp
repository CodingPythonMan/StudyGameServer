#include <iostream>
#include <list>
using namespace std;

struct alignas(32) DATA1 {
	char a;
	short b;
	int c;
	int d[8];
};

struct DATA2 {
	char a;
	short b;
	alignas(32) int c;
	int d[7];
};

struct User {
	char name[50];
	int a;
	int b;

	User()
	{
		memset(name, 0, 50);
		a = 12;
		b = 30;
	}
};

int Test(int a, int b)
{
	return 8;
}

int main()
{
	cout << "DATA1 sizeof : " << sizeof(DATA1) << "\n";
	cout << "DATA2 sizeof : " << sizeof(DATA2) << "\n";

	DATA1 data1;
	data1.a = 0;
	data1.b = 0;
	data1.c = 0;

	DATA2 data2;
	data2.a = 0;
	data2.b = 0;
	data2.c = 0;

	//int test = Test(10, 20);
	list<User*> userList;

	User* user = new User;
	userList.push_back(user);

	for (auto iter = userList.begin(); iter != userList.end(); ++iter)
		delete (*iter);
}