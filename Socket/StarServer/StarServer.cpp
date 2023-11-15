#include "StarServer.h"

SOCKET listenSock;
MyList<Session*> ClientList;
MyList<Session*> DeleteList;
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
	FD_SET rset, wset;
	int retval;

	Rendering();

	while (1)
	{
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listenSock, &rset);

		MyList<Session*>::iterator iter;
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			FD_SET((*iter)->Sock, &rset);

			// 세션 SendQ 사용 여부에 따라 wset 설정
			if ((*iter)->sendBuffer.GetUseSize() > 0)
			{
				FD_SET((*iter)->Sock, &wset);
			}
		}

		// 무한 대기
		retval = select(0, &rset, &wset, nullptr, nullptr);
		if (retval == SOCKET_ERROR)
			return true;

		// 리슨소켓 검사
		if (FD_ISSET(listenSock, &rset))
		{
			AcceptProc();
		}

		// 데이터 통신
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			if (FD_ISSET((*iter)->Sock, &rset))
			{
				// 데이터 받기
				ReadProc(*iter);
			}

			if (FD_ISSET((*iter)->Sock, &wset))
			{
				// 데이터 쓰기
				WriteProc(*iter);
			}
		}

		// 삭제할 Client 삭제
		DeleteExecute();

		// 랜더링
		Rendering();
	}

	return false;
}

bool EndSocket()
{
	closesocket(listenSock);

	WSACleanup();

	return false;
}

void AcceptProc()
{
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET)
		return;

	Session* session = new Session;
	session->Sock = clientSock;
	session->ID = UniqueID;
	session->X = MAX_X / 2;
	session->Y = MAX_Y / 2;

	InetNtop(AF_INET, &(clientAddr.sin_addr), session->IP, 16);
	session->Port = ntohs(clientAddr.sin_port);
	ClientList.push_back(session);
	
	// 1. Send AssignID(당사자), StarCreate(전체)
	AssignID assignID;
	assignID.ID = UniqueID;
	assignID.Type = (int)MessageType::AssignID;
	SendUnicast(session, (char*)&assignID);
	
	CreateStar createStar;
	createStar.ID = UniqueID;
	createStar.Type = (int)MessageType::CreateStar;
	createStar.X = MAX_X / 2;
	createStar.Y = MAX_Y / 2;
	SendBroadcast(nullptr, (char*)&createStar);

	// 2. 다른 사람의 CreateStar 도 받아야 한다.
	MyList<Session*>::iterator iter;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		if (session == *iter)
			continue;

		Session* otherSession = *iter;
		createStar.ID = otherSession->ID;
		createStar.X = otherSession->X;
		createStar.Y = otherSession->Y;
		SendUnicast(session, (char*)&createStar);
	}

	UniqueID++;
}

void ReadProc(Session* session)
{
	// 데이터 받은 후 처리 필요
	char buf[RINGBUFFER_MAX];
	int retval;

	retval = recv(session->Sock, buf, RINGBUFFER_MAX, 0);
	if (retval == SOCKET_ERROR)
	{
		retval = GetLastError();

		if (retval == WSAEWOULDBLOCK)
			return;
		else if (retval == WSAECONNRESET)
		{
			Disconnect(session);
			return;
		}
	}
	else if (retval == 0)
	{
		Disconnect(session);
		return;
	}

	if(session->recvBuffer.GetFreeSize() > retval)
		session->recvBuffer.Enqueue(buf, retval);

	while(1)
	{
		if (session->recvBuffer.GetUseSize() < 16)
		{
			break;
		}

		char message[16];
		retval = session->recvBuffer.Dequeue(message, 16);
		if (retval != 16)
		{
			return;
		}

		int type = *((int*)buf);
		switch ((MessageType)type)
		{
		case MessageType::MoveStar:
		{
			MoveStar moveStar;
			// 1. 받고 랜더링 위한 좌표 설정
			moveStar.Type = ((MoveStar*)buf)->Type;
			moveStar.ID = ((MoveStar*)buf)->ID;
			moveStar.X = ((MoveStar*)buf)->X;
			moveStar.Y = ((MoveStar*)buf)->Y;

			if (moveStar.X < 0 || moveStar.Y < 0 || moveStar.X >= MAX_X || moveStar.Y >= MAX_Y)
				break;
			
			session->X = moveStar.X;
			session->Y = moveStar.Y;

			// 2. 다른 이에게 BroadCast
			SendBroadcast(session, (char*)&moveStar);

			// 랜더링
			Rendering();
		}
		break;
		default:
			Disconnect(session);
			break;
		}
	}
}

void WriteProc(Session* session)
{
	char buffer[RINGBUFFER_MAX];

	while (1)
	{
		if (session->sendBuffer.GetUseSize() < 16)
			break;

		session->sendBuffer.Peek(buffer, 16);
		int retval = send(session->Sock, buffer, 16, 0);
		if (retval == SOCKET_ERROR)
		{
			retval = GetLastError();

			if (retval == WSAEWOULDBLOCK)
				return;
			else if (retval == WSAECONNRESET)
			{
				Disconnect(session);
				return;
			}
		}
		
		// 자랑스러운 로그. 해당 로그 기록으로 RingBuffer 버그를 잡았다.
		//sprintf_s(buf, 40, "[ID:%d] Type:%d, Size:%d\n", session->ID, (int)(*buffer), retval);

		session->sendBuffer.MoveFront(retval);
	}
}

void SendUnicast(Session* session, char* buf)
{
	if(session->sendBuffer.GetFreeSize() > 16)
		session->sendBuffer.Enqueue(buf, 16);
}

void SendBroadcast(Session* session, char* buf)
{
	MyList<Session*>::iterator iter;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		if (*iter == session)
			continue;

		if ((*iter)->sendBuffer.GetFreeSize() > 16)
			(*iter)->sendBuffer.Enqueue(buf, 16);
	}
}

void Disconnect(Session* session)
{
	// 1. 별삭제 패킷 전송
	DeleteStar deleteStar;
	deleteStar.ID = session->ID;
	deleteStar.Type = (int)MessageType::DeleteStar;
	SendBroadcast(session, (char*)&deleteStar);

	// 2. ClientList 에서 삭제 => 우선 임시로 삭제 리스트에 추가
	DeleteList.push_back(session);
}

void DeleteExecute()
{
	MyList<Session*>::iterator iter;
	for (iter = DeleteList.begin(); iter != DeleteList.end(); ++iter)
	{
		closesocket((*iter)->Sock);
		ClientList.remove(*iter);
		delete(*iter);
	}

	DeleteList.clear();
}

void Rendering()
{
	MyList<Session*>::iterator iter;
	Clear();
	char message[25] = "Client Connect : \0";
	int count = 0;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		SpriteDraw((*iter)->X, (*iter)->Y, '*');
		count++;
	}
	snprintf(&message[(int)strlen(message)], strlen(message), "%d", count);
	memcpy(&_ScreenBuffer[0][0], message, strlen(message));
	Flip();
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

void WriteLog(char* buf)
{
	char fileName[70] = "Logs.txt";
	tm t;
	time_t timer;

	timer = time(NULL);    // 현재 시각을 초 단위로 얻기
	localtime_s(&t, &timer); // 초 단위의 시간을 분리하여 구조체에 넣기

	FILE* file;

	fopen_s(&file, fileName, "a+");
	if (file == nullptr)
		return;

	fwrite(buf, 1, (int)strlen(buf), file);

	fclose(file);
}