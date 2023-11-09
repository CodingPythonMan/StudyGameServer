#include "StarServer.h"

struct Session {
	SOCKET Sock;
	int ID;
	int X;
	int Y;
	WCHAR IP[16];
	int Port;
};

SOCKET listenSock;
MyList<Session*> ClientList;
int UniqueID = 0;

bool SetServerSocket()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return true;

	// 리슨 소켓
	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return true;

	// bind
	SOCKADDR_IN listenAddr;
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(SERVER_PORT);
	retval = bind(listenSock, (SOCKADDR*)&listenAddr, sizeof(listenAddr));

	if (retval == SOCKET_ERROR)
		return true;

	return false;
}

bool ListenSocket()
{
	// listen
	int retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return true;

	// 논블로킹 소켓으로 전환
	u_long on = 1;
	retval = ioctlsocket(listenSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
		return true;

	return false;
}

bool SelectLoop()
{
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	char buf[16];
	SOCKADDR_IN clientAddr;
	SOCKET clientSock;
	int addrLen;

	int retval;

	while (1)
	{
		FD_ZERO(&rset);
		FD_SET(listenSock, &rset);
		MyList<Session*>::iterator iter;
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			FD_SET((*iter)->Sock, &rset);
		}

		// 무한 대기
		retval = select(0, &rset, nullptr, nullptr, nullptr);
		if (retval == SOCKET_ERROR)
			return true;

		// 리슨소켓 검사
		if (FD_ISSET(listenSock, &rset))
		{
			addrLen = sizeof(clientAddr);
			clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSock == INVALID_SOCKET)
				return true;

			AcceptProc(&clientSock, &clientAddr);
		}

		// 데이터 통신

	}

	return false;
}

bool EndSocket()
{
	closesocket(listenSock);

	WSACleanup();

	return false;
}

void AcceptProc(SOCKET* clientSock, SOCKADDR_IN* clientAddr)
{
	Session* session = new Session;
	session->Sock = *clientSock;
	session->ID = UniqueID;
	
	session->X = MAX_X / 2;
	session->Y = MAX_Y / 2;
	InetNtop(AF_INET, &(clientAddr->sin_addr), session->IP, 16);
	session->Port = ntohs(clientAddr->sin_port);
	ClientList.push_back(session);

	// Send AssignID(당사자), StarCreate(전체)
	
	AssignID assignID;
	assignID.ID = UniqueID;
	assignID.Type = (int)MessageType::AssignID;
	
	
	CreateStar createStar;
	createStar.ID = UniqueID;
	createStar.Type = (int)MessageType::AssignID;
	createStar.X = MAX_X / 2;
	createStar.Y = MAX_Y / 2;
	SendBroadCast(session, (char*)&createStar);

	UniqueID++;
}

void SendUnicast(Session* session, char* buf)
{
	int retval = send(session->Sock, buf, retval, 0);
	if (retval == SOCKET_ERROR)
	{
		Disconnect();
	}
}

void SendBroadCast(Session* session, char* buf)
{
	int retval;

	MyList<Session*>::iterator iter;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		if (*iter == session)
			continue;

		retval = send(session->Sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{

		}
	}
}

void Disconnect()
{

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