#include "StarServer.h"

SOCKET listenSock;
MyList<Session*> ClientList;
MyList<Session*> DeleteList;
int UniqueID = 0;

bool SetServerSocket()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return true;
	}

	// ���� ����
	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
	{
		return true;
	}

	// bind
	SOCKADDR_IN listenAddr;
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(SERVER_PORT);
	retval = bind(listenSock, (SOCKADDR*)&listenAddr, sizeof(listenAddr));

	if (retval == SOCKET_ERROR)
	{
		return true;
	}

	return false;
}

bool ListenSocket()
{
	// listen
	int retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		return true;
	}

	// ����ŷ �������� ��ȯ
	u_long on = 1;
	retval = ioctlsocket(listenSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
	{
		return true;
	}

	return false;
}

bool SelectLoop()
{
	// ������ ��ſ� ����� ����
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

			// ���� SendQ ��� ���ο� ���� wset ����
			if ((*iter)->sendBuffer.GetUseSize() > 0)
			{
				FD_SET((*iter)->Sock, &wset);
			}
		}

		// ���� ���
		retval = select(0, &rset, &wset, nullptr, nullptr);
		if (retval == SOCKET_ERROR)
		{
			return true;
		}

		// �������� �˻�
		if (FD_ISSET(listenSock, &rset))
		{
			AcceptProc();
		}

		// ������ ���
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			if (FD_ISSET((*iter)->Sock, &rset))
			{
				// ������ �ޱ�
				ReadProc(*iter);
			}

			if (FD_ISSET((*iter)->Sock, &wset))
			{
				// ������ ����
				WriteProc(*iter);
			}
		}

		// ������ Client ����
		DeleteExecute();

		// ������
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
	
	// 1. Send AssignID(�����), StarCreate(��ü)
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

	// 2. �ٸ� ����� CreateStar �� �޾ƾ� �Ѵ�.
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
	// ������ ���� �� ó�� �ʿ�
	int retval;

	int recvAvailableSize = session->recvBuffer.DirectEnqueueSize();
	if (recvAvailableSize < 16)
	{
		char buffer[16];
		retval = recv(session->Sock, buffer, 16, 0);
		
		if (session->recvBuffer.GetFreeSize() > 16)
			session->recvBuffer.Enqueue(buffer, retval);
	}
	else
	{
		char* ptr = session->recvBuffer.GetRearBufferPtr();
		retval = recv(session->Sock, ptr, recvAvailableSize, 0);
		session->recvBuffer.MoveRear(retval);
	}

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

		int type = *((int*)message);
		switch ((MessageType)type)
		{
		case MessageType::MoveStar:
		{
			MoveStar moveStar;
			// 1. �ް� ������ ���� ��ǥ ����
			moveStar.Type = ((MoveStar*)message)->Type;
			moveStar.ID = ((MoveStar*)message)->ID;
			moveStar.X = ((MoveStar*)message)->X;
			moveStar.Y = ((MoveStar*)message)->Y;

			if (moveStar.X < 0 || moveStar.Y < 0 || moveStar.X >= MAX_X || moveStar.Y >= MAX_Y)
				break;
			
			session->X = moveStar.X;
			session->Y = moveStar.Y;

			// 2. �ٸ� �̿��� BroadCast
			SendBroadcast(session, (char*)&moveStar);

			// ������
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
	int retval;

	while (1)
	{
		if (session->sendBuffer.GetUseSize() < 16)
			break;

		// DirectDequeue �� �����ۿ��� Tcp �۽Ź��۷� ���� �־��ִ� ����
		if (session->sendBuffer.DirectDequeueSize() < 16)
		{
			char buffer[16];
			session->sendBuffer.Peek(buffer, 16);
			retval = send(session->Sock, buffer, 16, 0);
		}
		else
		{
			char* ptr = session->sendBuffer.GetFrontBufferPtr();
			retval = send(session->Sock, ptr, 16, 0);
		}
		
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
		
		// �ڶ������� �α�. �ش� �α� ������� RingBuffer ���׸� ��Ҵ�.
		//char buf[40];
		//sprintf_s(buf, 40, "[ID:%d] Size:%d\n", session->ID, retval);
		//WriteLog(buf);

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
	// 1. ������ ��Ŷ ����
	DeleteStar deleteStar;
	deleteStar.ID = session->ID;
	deleteStar.Type = (int)MessageType::DeleteStar;
	SendBroadcast(session, (char*)&deleteStar);

	// 2. ClientList ���� ���� => �켱 �ӽ÷� ���� ����Ʈ�� �߰�
	DeleteList.push_back(session);
}

void DeleteExecute()
{
	MyList<Session*>::iterator iter;
	for (iter = DeleteList.begin(); iter != DeleteList.end(); ++iter)
	{
		closesocket((*iter)->Sock);
		ClientList.remove(*iter);
		// remove ���� delete ó������ ���ְ� �����Ƿ�, delete �� �� ���� �ʿ�� ����.
		//delete(*iter);
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

	timer = time(NULL);    // ���� �ð��� �� ������ ���
	localtime_s(&t, &timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�

	FILE* file;

	fopen_s(&file, fileName, "a+");
	if (file == nullptr)
		return;

	fwrite(buf, 1, (int)strlen(buf), file);

	fclose(file);
}