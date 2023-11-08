#include "StarClient.h"

SOCKET sock;

struct Star {
	int id;
	bool use;
	int x;
	int y;
};

Star stars[MAX_STARS];
Star* myStar;

bool SetClientSocket()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return true;

	// Socket Ready
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return true;

    return false;
}

bool ConnectClientSocket(WCHAR IP[])
{
	// connect
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, IP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return true;

    return false;
}

bool TransSocket()
{
	// 블락 논블락은 언제든 변할 수 있다.
	// connect 후 논 블락으로 전환
	u_long on = 1;
	int retval = ioctlsocket(sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
		return true;

    return false;
}

bool SelectLoop()
{
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	char buf[16];

	int retval;

	while (1)
	{
		// 네트워크 : FD_SET ReadSet
		FD_ZERO(&rset);
		FD_SET(sock, &rset);

		// select
		const timeval TimeVal{ 0, 0 };
		retval = select(0, &rset, nullptr, nullptr, &TimeVal);
		if (retval == SOCKET_ERROR)
			return true;

		// 소켓 셋 검사
		if (FD_ISSET(sock, &rset))
		{
			// 데이터 받기
			retval = recv(sock, buf, 16, 0);
			if (retval == SOCKET_ERROR)
			{
				// 연결 끊김. 종료
				return true;
			}
			else if (retval == 0)
			{
				return true;
			}

			// 받은 데이터 정리
			int type = *((int*)buf);
			switch ((MessageType)type)
			{
			case MessageType::AssignID:
			{
				AssignID* assignID = (AssignID*)buf;
				for (int i = 0; i < MAX_STARS; i++)
				{
					if (stars[i].use == false)
					{
						myStar = &stars[i];
						myStar->id = assignID->ID;
						myStar->use = true;
						myStar->x = MAX_X / 2;
						myStar->y = MAX_Y / 2;
						break;
					}
				}
				break;
			}
			case MessageType::CreateStar:
			{
				CreateStar* createStar = (CreateStar*)buf;
				if (myStar != nullptr && myStar->id == createStar->ID)
					break;
					
				for (int i = 0; i < MAX_STARS; i++)
				{
					if (stars[i].use == false)
					{
						stars[i].id = createStar->ID;
						stars[i].x = createStar->X;
						stars[i].y = createStar->Y;
						stars[i].use = true;
						break;
					}
				}
				break;
			}
			case MessageType::DeleteStar:
			{
				DeleteStar* deleteStar = (DeleteStar*)buf;
				for (int i = 0; i < MAX_STARS; i++)
				{
					if (stars[i].id == deleteStar->ID)
					{
						stars[i].use = false;
						break;
					}
				}
				break;
			}
			case MessageType::MoveStar:
			{
				MoveStar* moveStar = (MoveStar*)buf;
				if (moveStar->X < 0 || moveStar->Y < 0 || moveStar->X >= MAX_X || moveStar->Y >= MAX_Y)
					break;

				for (int i = 0; i < MAX_STARS; i++)
				{
					if (stars[i].id == moveStar->ID)
					{
						stars[i].x = moveStar->X;
						stars[i].y = moveStar->Y;
					}
				}
				break;
			}
			default:
				return true;
			}
		}

		// 송신 : 좌표 변경되었다면, MoveStar send 한다.
		if (myStar != nullptr)
		{
			bool edit = false;
			// 왼쪽 방향키.
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (myStar->x > 0)
					myStar->x--;

				edit = true;
			}
			// 오른쪽 방향키.
			if (GetAsyncKeyState(VK_RIGHT))
			{
				if (myStar->x < MAX_X-2)
					myStar->x++;

				edit = true;
			}
			// 위쪽 방향키.
			if (GetAsyncKeyState(VK_UP) & 0x8001)
			{
				if (myStar->y > 0)
					myStar->y--;

				edit = true;
			}
			// 아래쪽 방향키.
			if (GetAsyncKeyState(VK_DOWN) & 0x8001)
			{
				if (myStar->y+1 < MAX_Y-1)
					myStar->y++;

				edit = true;
			}

			if (edit == true)
			{
				MoveStar moveStar;
				moveStar.Type = (int)MessageType::MoveStar;
				moveStar.ID = myStar->id;
				moveStar.X = myStar->x;
				moveStar.Y = myStar->y;
				retval = send(sock, (char*)&moveStar, sizeof(MoveStar), 0);
			}
		}
		
		// 랜더링
		Clear();
		for (int i = 0; i < MAX_STARS; i++)
		{
			if (stars[i].use == true)
			{
				SpriteDraw(stars[i].x, stars[i].y, '*');
			}
		}
		Flip();
	}

	return false;
}

bool EndSocket()
{
	closesocket(sock);

	WSACleanup();

	return true;
}

void Flip()
{
	for (int count = 0; count < MAX_Y; count++)
	{
		MoveCursor(0, count);
		printf(_ScreenBuffer[count]);
	}
}

void Clear()
{
	for (int count = 0; count < MAX_Y; count++)
	{
		memset(_ScreenBuffer[count], ' ', MAX_X);
		_ScreenBuffer[count][MAX_X - 1] = (char)NULL;
	}
}

void SpriteDraw(int X, int Y, char Sprite)
{
	if (X < 0 || Y < 0 || X >= MAX_X - 1 || Y >= MAX_Y)
		return;

	_ScreenBuffer[Y][X] = Sprite;
}