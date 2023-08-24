#pragma once

//--------------------------------------------------------------------
// ȭ�� �������� ���ֱ� ���� ȭ�� ����.
// ������ ����Ǵ� ��Ȳ�� �Ź� ȭ���� ����� ����� ���, ����� ���,
// �ϰ� �Ǹ� ȭ���� �������� �Ÿ��� �ȴ�.
//
// �׷��Ƿ� ȭ��� �Ȱ��� ũ���� �޸𸮸� �Ҵ��� ������ ȭ�鿡 �ٷ� �����ʰ�
// �޸�(����)�� �׸��� �׸��� �޸��� ȭ���� �״�� ȭ�鿡 ����ش�.
//
// �̷��� �ؼ� ȭ���� �Ź� �����, �׸���, �����, �׸��� ���� �ʰ�
// �޸�(����)���� �׸��� ȭ�鿡 �׸��� �۾��� �ϰ� �Ǿ� �������� ��������.
//
// ������ �� �� �������� NULL �� �־� ���ڿ��μ� ó���ϸ�, 
// ���������� printf �� ���� ���̴�.
//
// for ( N = 0 ~ height )
// {
// 	  cs_MoveCursor(0, N);
//    printf(szScreenBuffer[N]);
// }
//
// �ٹٲ޿� printf("\n") �� ���� �ʰ� Ŀ����ǥ�� �̵��ϴ� ������
// ȭ���� �� ���� ����ϰ� �ٹٲ��� �ϸ� 2ĭ�� �������ų� ȭ���� �и� �� �����Ƿ�
// �� �� ��¸��� ��ǥ�� ������ �̵��Ͽ� Ȯ���ϰ� ����Ѵ�.
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// GetAsyncKeyState(int iKey)  #include <Windows.h>
//
// ������ API �� Ű���尡 ���ȴ����� Ȯ���Ѵ�.
// ���ڷ� Ű���� ��ư�� ���� ������ ���� ������ �ش� Ű�� ���ȴ��� (���ȴ����� �ִ���) �� Ȯ�� ���ش�.
// ��� Ű������ Ȯ���� �����ϰ�, �񵿱�� üũ�� �ǹǷ� ���ӿ����� ���� ����.
//
// Virtual-Key Codes
//
// VK_SPACE / VK_ESCAPE / VK_LEFT / VK_UP / Ű���� ���ڴ� �빮�� �ƽ�Ű �ڵ�� ����.
// winuser.h ���Ͽ� ���� ���� ������ �Ǿ� �ִ�.
//
//
// GetAsyncKeyState(VK_LEFT) ȣ��� �������
//
// 0x0001  > *���� üũ ���� �������� ����
// 0x8000  > ���� ��������
// 0x8001  > *���� üũ ���� �������� �ְ� ���ݵ� ���� ����
//
// * ���� üũ��°� ������ GetAsyncKeyState �� ȣ���� ���� �� �Ѵ�.
// 
// 10������ ¥�� �����̶�� 1�ʿ� 10ȸ�� Ű üũ�� �ϰ� �ǹǷ� üũ ������ 20ms �� �ȴ�.
// ���� Ŀ�ǵ� �Է��� �ʿ��� ���ӿ����� 20ms �̳��� �������� Ű�Է��� �ִٸ� üũ���� ���ϴ� Ű �Է��� �߻� �� �� �ִ�.
// �׷��� 0x0001 ��Ʈ�� ���� ó���� �ʿ��ϴ�.
//
// Ű�� ���� �� ���� 1ȸ�� üũ �ϰ��� �Ѵٸ� �߰� ������ �Ҵ��Ͽ� �˾Ƽ� �ذ� �ؾ� ��!
//
char ScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

//--------------------------------------------------------------------
// �Ʊ� ����ü ����
//--------------------------------------------------------------------
struct _PLAYER
{
	int HP;					// �÷��̾� HP

	int X;						// �÷��̾� ��ǥ
	int Y;

	int Visible;				// �÷��̾� ���� ����.

};

//--------------------------------------------------------------------
// �÷��̾� �ν��Ͻ� ����, (1�ο� �̴� �ϳ��� ����)
//--------------------------------------------------------------------
_PLAYER _Player;

//--------------------------------------------------------------------
// �̻��� ����ü ����
//--------------------------------------------------------------------
struct _Missile
{
	int X;
	int Y;

	int Visible;
};

//--------------------------------------------------------------------
// �̻��� ����
//--------------------------------------------------------------------
_Missile _Missiles[SCREEN_HEIGHT * SCREEN_WIDTH];


//--------------------------------------------------------------------
// ���� �̻��� ��
//--------------------------------------------------------------------
int _MissileCount = 0;

//--------------------------------------------------------------------
// ���� ����ü ����
//--------------------------------------------------------------------
struct _Monster
{
	int X;
	int Y;

	int Visible;
};

//--------------------------------------------------------------------
// ���� �ν��Ͻ� ����,
//--------------------------------------------------------------------
_Monster _Monsters[20];

//--------------------------------------------------------------------
// ���� ���� ��.
//--------------------------------------------------------------------
int _MonsterCount = 0;


//--------------------------------------------------------------------
// �������� �� �迭
//--------------------------------------------------------------------
const int _StageCount = 3;
const char* _Stages[_StageCount] = {"Stage1.data", "Stage2.data", "Stage3.data"};


//--------------------------------------------------------------------
// ���� ���۽� ��ü ������ �ʱ�ȭ��.
//--------------------------------------------------------------------
void Player_Initial(void);


//////////////////////////////////////////////////////////////////////
//
// ��º� �Լ���....
//
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
// ������ ������ ȭ������ ����ִ� �Լ�.
//
// ����,�Ʊ�,�Ѿ� ���� szScreenBuffer �� �־��ְ�, 
// 1 �������� ������ �������� �� �Լ��� ȣ���Ͽ� ���� -> ȭ�� ���� �׸���.
//--------------------------------------------------------------------
void Buffer_Flip(void);
//--------------------------------------------------------------------
// ȭ�� ���۸� �����ִ� �Լ�
//
// �� ������ �׸��� �׸��� ������ ���۸� ���� �ش�. 
// �ȱ׷��� ���� �������� �ܻ��� �����ϱ�
//--------------------------------------------------------------------
void Buffer_Clear(void);

//--------------------------------------------------------------------
// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���.
//
// �Է� ���� X,Y ��ǥ�� �ƽ�Ű�ڵ� �ϳ��� ����Ѵ�. (���ۿ� �׸�)
//--------------------------------------------------------------------
void Sprite_Draw(int X, int Y, char chSprite);




//////////////////////////////////////////////////////////////////////
//
// �÷��̾� ó���� �Լ���
//
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
// Ű�����Է��� �޾Ƽ� �÷��̾ ó���Ѵ�.
//
// ESC �� ���� ��� false ����, (����ó��)
//--------------------------------------------------------------------
bool KeyProcess(void);

//--------------------------------------------------------------------
// ��ũ�� ���ۿ� �÷��̾� �׸���
//--------------------------------------------------------------------
void Draw_Player(void);

//--------------------------------------------------------------------
// ��ũ�� ���ۿ� �̻��� �̵�
//--------------------------------------------------------------------
void Missile_Move(void);

//--------------------------------------------------------------------
// ��ũ�� ���ۿ� �̻��� �׸���
//--------------------------------------------------------------------
void Draw_Missile(void);

//--------------------------------------------------------------------
// ���� ���� ����
//--------------------------------------------------------------------
void Monster_Set(int stage);

//--------------------------------------------------------------------
// ���� �׸���
//--------------------------------------------------------------------
void Draw_Monster(void);