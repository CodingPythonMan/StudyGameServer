#include "FighterServer.h"

FighterServer::FighterServer()
{
	listenSock = NULL;
	TotalClientSocks = 0;
	UniqueID = 0;
}

FighterServer::~FighterServer()
{

}

bool FighterServer::Init()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return true;

	// 리슨 소켓 셋팅
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

	// listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return true;

	return false;
}

bool FighterServer::SelectLoop()
{
	// 데이터 통신에 사용할 변수
	FD_SET rset, wset;
	int retval;

	while (1)
	{
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listenSock, &rset);

		MyList<Session*>::iterator iter;
		for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
		{
			FD_SET((*iter)->Sock, &rset);

			// 세션 SendBuffer 에 따라 wset 설정
			if ((*iter)->SendBuffer.GetUseSize() > 0)
			{
				FD_SET((*iter)->Sock, &wset);
			}
		}

		// 이젠 Select 가 무한 대기가 되면 안 된다.
		// 프레임 서버로 간다.
		timeval time{ 0, 0 };
		retval = select(0, &rset, &wset, nullptr, &time);
		if (retval == SOCKET_ERROR)
			return true;

		// 리슨소켓 검사
		if (FD_ISSET(listenSock, &rset))
		{
			AcceptProc();
		}
	}

	return false;
}

void FighterServer::AcceptProc()
{
	SOCKADDR_IN clientAddr;
	int addLen = sizeof(clientAddr);
	SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addLen);
	if (clientSock == INVALID_SOCKET)
		return;

	Session* session = new Session;
	session->Sock = clientSock;
	session->ID = UniqueID;


	FIGHTER_CMD_CREATE_MY_CHARACTER
}

void FighterServer::Close() 
{
	closesocket(listenSock);
}