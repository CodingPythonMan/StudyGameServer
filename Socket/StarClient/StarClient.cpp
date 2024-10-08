#include "StarClient.h"
#include <fstream>
#include <iostream>

SOCKET g_sock;

struct Star {
	int id;
	bool use;
	int x;
	int y;
};

Star g_stars[MAX_STARS];
Star* g_myStar;

std::string filePath = "error.txt";

bool SetClientSocket()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return true;
	}

	// Socket Ready
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET)
	{
		return true;
	}

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
	int retval = connect(g_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		return true;
	}

    return false;
}

bool TransSocket()
{
	// 블락 논블락은 언제든 변할 수 있다.
	// connect 후 논 블락으로 전환
	u_long on = 1;
	int retval = ioctlsocket(g_sock, FIONBIO, &on);
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
		FD_SET(g_sock, &rset);

		// select
		const timeval TimeVal{ 0, 0 };
		retval = select(0, &rset, nullptr, nullptr, &TimeVal);
		if (retval == SOCKET_ERROR)
		{
			int errCode = WSAGetLastError();

			std::ofstream writeFile(filePath.data());
			if (writeFile.is_open())
			{
				writeFile << "에러 종료\n";
				writeFile << errCode;
				writeFile.close();
			}

			return true;
		}

		// 소켓 셋 검사
		if (FD_ISSET(g_sock, &rset))
		{
			// 데이터 받기
			retval = recv(g_sock, buf, 16, 0);
			if (retval == SOCKET_ERROR)
			{
				// 에러 연결 끊김. 종료
				// 간단한 파일로그
				int errCode = WSAGetLastError();

				std::ofstream writeFile(filePath.data());
				if (writeFile.is_open())
				{
					writeFile << "에러 종료\n";
					writeFile << errCode;
					writeFile.close();
				}

				return true;
			}
			else if (retval == 0)
			{
				// recv 에서 rst 받게 되면, 접속 종료하는 게 맞다.
				// 플레이 중에 서버의 잘못으로 이걸 받는 것이면, 파일로그로 남겨서 증거를 챙겨도 된다.

				std::ofstream writeFile(filePath.data());
				if (writeFile.is_open())
				{
					writeFile << "정상 종료\n";
					writeFile.close();
				}

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
					if (g_stars[i].use == false)
					{
						g_myStar = &g_stars[i];
						g_myStar->id = assignID->ID;
						g_myStar->use = true;
						g_myStar->x = MAX_X / 2;
						g_myStar->y = MAX_Y / 2;
						break;
					}
				}
				break;
			}
			case MessageType::CreateStar:
			{
				CreateStar* createStar = (CreateStar*)buf;
				if (g_myStar != nullptr && g_myStar->id == createStar->ID)
				{
					break;
				}
					
				for (int i = 0; i < MAX_STARS; i++)
				{
					if (false == g_stars[i].use)
					{
						g_stars[i].id = createStar->ID;
						g_stars[i].x = createStar->X;
						g_stars[i].y = createStar->Y;
						g_stars[i].use = true;
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
					if (g_stars[i].id == deleteStar->ID)
					{
						g_stars[i].use = false;
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
					if (g_stars[i].id == moveStar->ID)
					{
						g_stars[i].x = moveStar->X;
						g_stars[i].y = moveStar->Y;
					}
				}
				break;
			}
			default:
			{
				std::ofstream writeFile(filePath.data());
				if (writeFile.is_open())
				{
					writeFile << "에러 종료\n";
					writeFile << type;
					writeFile.close();
				}

				return true;
			}
			}
		}

		// 송신 : 좌표 변경되었다면, MoveStar send 한다.
		if (g_myStar != nullptr)
		{
			bool edit = false;
			// 왼쪽 방향키.
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (g_myStar->x > 0)
				{
					g_myStar->x--;
				}

				edit = true;
			}
			// 오른쪽 방향키.
			if (GetAsyncKeyState(VK_RIGHT))
			{
				if (g_myStar->x < MAX_X - 2)
				{
					g_myStar->x++;
				}

				edit = true;
			}
			// 위쪽 방향키.
			if (GetAsyncKeyState(VK_UP) & 0x8001)
			{
				if (g_myStar->y > 0)
				{
					g_myStar->y--;
				}

				edit = true;
			}
			// 아래쪽 방향키.
			if (GetAsyncKeyState(VK_DOWN) & 0x8001)
			{
				if (g_myStar->y + 1 < MAX_Y - 1)
				{
					g_myStar->y++;
				}

				edit = true;
			}

			if (edit == true)
			{
				MoveStar moveStar;
				moveStar.Type = (int)MessageType::MoveStar;
				moveStar.ID = g_myStar->id;
				moveStar.X = g_myStar->x;
				moveStar.Y = g_myStar->y;
				retval = send(g_sock, (char*)&moveStar, sizeof(MoveStar), 0);
			}
		}
		
		// 랜더링
		Clear();
		for (int i = 0; i < MAX_STARS; i++)
		{
			if (g_stars[i].use == true)
			{
				SpriteDraw(g_stars[i].x, g_stars[i].y, '*');
			}
		}
		Flip();

		Sleep(50);
	}

	return false;
}

bool EndSocket()
{
	closesocket(g_sock);

	WSACleanup();

	return false;
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