//////////////////////////////////////////////////////////////
// Windows �� �ܼ� ȭ�鿡�� Ŀ�� ����.
//
//////////////////////////////////////////////////////////////
#ifndef __CONSOLE__
#define __CONSOLE__

#define SCREEN_WIDTH	81		// �ܼ� ���� 80ĭ + NULL
#define SCREEN_HEIGHT	24		// �ܼ� ���� 25ĭ + NULL
//-------------------------------------------------------------
// �ܼ� ��� ���� �غ� �۾�.
//
//-------------------------------------------------------------
void cs_Initial(void);

//-------------------------------------------------------------
// �ܼ� ȭ���� Ŀ���� X, Y ��ǥ�� �̵���Ų��.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY);

//-------------------------------------------------------------
// �ܼ� ȭ���� ����ȭ �Ѵ�.
//
//-------------------------------------------------------------
void cs_ClearScreen(void);


#endif
