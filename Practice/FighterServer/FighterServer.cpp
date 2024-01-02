#include "FighterServer.h"
#include "Packet.h"
#include <cstdio>

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

	// linger 옵션 설정
	linger Linger;
	Linger.l_onoff = 1;
	Linger.l_linger = 0;
	setsockopt(listenSock, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(Linger));

	// listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return true;

	wprintf(L"[Listen Status] : OK\n");

	return false;
}

bool FighterServer::SelectLoop()
{
	// 데이터 통신에 사용할 변수
	FD_SET rset, wset;
	int retval;

	unsigned int tick = 0;
	unsigned int curTime = timeGetTime();
	unsigned int ourTime = curTime;

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

		// 데이터 통신
		for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
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

		// Disconnect 처리
		DeleteSessions();

		// 플레이어들 이동
		for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
		{
			(*iter)->_Player->MovePlayer();
		}
		
		// 프레임 계산 필요
		curTime = timeGetTime();
		tick = curTime - ourTime;
		ourTime += WAIT;

		if (tick <= WAIT)
		{
			Sleep(WAIT - tick);
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

	Direction FirstDirection = Direction::LL;

	Session* session = new Session;
	session->Sock = clientSock;
	session->ID = UniqueID;
	session->Port = ntohs(clientAddr.sin_port);
	InetNtop(AF_INET, &clientAddr.sin_addr, session->IP, 16);
	// Session 생성 알려주기
	wprintf(L"[Client Connect] ID : %d, IP : %s, Port : %d\n", 
		UniqueID, session->IP, session->Port);
	
	Player* player = new Player;
	session->_Player = player;

	// 1. 당사자에게 생성됐음을 알려주기
	Packet CreateMyChar;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = sizeof(int) + sizeof(unsigned char)
		+ sizeof(short) + sizeof(short) + sizeof(unsigned char);
	header.ByType = (unsigned char)PacketType::FIGHTER_CMD_CREATE_MY_CHARACTER;
	
	CreateMyChar.PutData((char*)&header, HEADER_SIZE);
	
	CreateMyChar << UniqueID;
	CreateMyChar << (unsigned char)FirstDirection;
	short X;
	short Y;
	unsigned char HP;
	player->NotifyPlayer(&X, &Y, &HP);
	CreateMyChar << X;
	CreateMyChar << Y;
	CreateMyChar << HP;
	SendUnicast(session, CreateMyChar.GetBufferPtr(), HEADER_SIZE+header.BySize);

	clientSocks.push_back(session);

	// 2. 다른 사람에게 알려주기
	Packet CreateOtherChar;
	header.ByType = (unsigned char)PacketType::FIGHTER_CMD_CREATE_OTHER_CHARACTER;

	CreateOtherChar.PutData((char*)&header, HEADER_SIZE);

	CreateOtherChar << UniqueID;
	CreateOtherChar << (unsigned char)FirstDirection;
	player->NotifyPlayer(&X, &Y, &HP);
	CreateOtherChar << X;
	CreateOtherChar << Y;
	CreateOtherChar << HP;
	SendBroadcast(session, CreateOtherChar.GetBufferPtr(), HEADER_SIZE+header.BySize);

	// 3. 다른 사람 위치 받기
	MyList<Session*>::iterator iter;
	for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
	{
		if (*iter == session)
			continue;

		CreateOtherChar.Clear();

		CreateOtherChar.PutData((char*)&header, HEADER_SIZE);

		CreateOtherChar << (*iter)->ID;
		CreateOtherChar << (unsigned char)(*iter)->_Player->_Direct;
		(*iter)->_Player->NotifyPlayer(&X, &Y, &HP);
		CreateOtherChar << X;
		CreateOtherChar << Y;
		CreateOtherChar << HP;
		SendUnicast(session, CreateOtherChar.GetBufferPtr(), HEADER_SIZE + header.BySize);
	}

	// UniqueID 증가
	UniqueID++;
}

void FighterServer::ReadProc(Session* session)
{
	// 데이터 받은 후 처리 필요
	int retval;

	int recvAvailableSize = session->RecvBuffer.DirectEnqueueSize();
	char* ptr = session->RecvBuffer.GetRearBufferPtr();
	retval = recv(session->Sock, ptr, recvAvailableSize, 0);
	session->RecvBuffer.MoveRear(retval);

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

	while (1)
	{
		// Peek 전에 오류 나버린다.
		if (session->RecvBuffer.GetUseSize() <= HEADER_SIZE)
			break;

		PACKET_HEADER header;
		session->RecvBuffer.Peek((char*)&header, HEADER_SIZE);
		if (session->RecvBuffer.GetUseSize() < HEADER_SIZE + header.BySize)
			break;

		// 프로토콜 코드가 맞지 않다면 내보낸다.
		if (header.ByCode != 0x89)
			break;

		// 마샬링
		char message[MAX_PACKET_SIZE];
		retval = session->RecvBuffer.Dequeue(message, HEADER_SIZE+header.BySize);

		PacketType type = (PacketType)(((PACKET_HEADER*)message)->ByType);
		switch (type)
		{
		case PacketType::FIGHTER_QRY_MOVE_START:
		{
			int pos = HEADER_SIZE;
			unsigned char Move = *(unsigned char*)(message + pos);
			pos += sizeof(unsigned char);
			short X = *(short*)(message + pos);
			pos += sizeof(X);
			short Y = *(short*)(message + pos);
			
			session->_Player->_MoveType = (MoveType)Move;
			if (true == session->_Player->MovePos(X, Y, true))
			{
				Disconnect(session);
				break;
			}

			Packet packet;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(unsigned char) 
				+ sizeof(short) + sizeof(short);
			header.ByType = (unsigned char)PacketType::FIGHTER_REP_MOVE_START;
			
			packet.PutData((char*)&header, HEADER_SIZE);

			packet << session->ID;
			packet << Move;
			packet << X;
			packet << Y;

			// 이 유저가 움직이기 시작했다는 것을 모두에게 알려줘야 한다.
			SendBroadcast(session, packet.GetBufferPtr(), HEADER_SIZE + header.BySize);
		
			// 로그
			wprintf(L"[Move Start] ID : %d, X : %d, Y : %d, Direction : %d \n", 
				session->ID, X, Y, session->_Player->_MoveType);
		}
		break;
		case PacketType::FIGHTER_QRY_MOVE_STOP:
		{
			int pos = HEADER_SIZE;
			unsigned char Direct = *(unsigned char*)(message + pos);
			pos += sizeof(unsigned char);
			short X = *(short*)(message + pos);
			pos += sizeof(X);
			short Y = *(short*)(message + pos);

			session->_Player->_Direct = (Direction)Direct;
			if(true == session->_Player->MovePos(X, Y, false))
			{
				Disconnect(session);
				break;
			}

			Packet packet;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(unsigned char) 
				+ sizeof(short) + sizeof(short);
			header.ByType = (unsigned char)PacketType::FIGHTER_REP_MOVE_STOP;

			packet.PutData((char*)&header, HEADER_SIZE);

			packet << session->ID;
			packet << Direct;
			packet << X;
			packet << Y;

			// 이 유저가 움직이기 멈췄다는 것을 모두에게 알려줘야 한다.
			SendBroadcast(session, packet.GetBufferPtr(), HEADER_SIZE + header.BySize);

			// 로그
			wprintf(L"[Move Stop] ID : %d, X : %d, Y : %d, Direct : %d \n", 
				session->ID, X, Y, session->_Player->_Direct);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_001:
		{
			int pos = HEADER_SIZE;
			unsigned char Direct = *(unsigned char*)(message + pos);
			pos += sizeof(Direct);
			short X = *(short*)(message + pos);
			pos += sizeof(X);
			short Y = *(short*)(message + pos);

			session->_Player->_Direct = (Direction)Direct;

			Packet packet;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(unsigned char) + sizeof(short) + sizeof(short);
			header.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_001;

			packet.PutData((char*)&header, HEADER_SIZE);

			packet << session->ID;
			packet << Direct;
			packet << X;
			packet << Y;
			session->_Player->NotifyPlayer(&X, &Y, nullptr);
			SendBroadcast(session, packet.GetBufferPtr(), HEADER_SIZE + header.BySize);

			// 충돌처리
			CheckDamage(session, ATTACK_TYPE::ATTACK001);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_002:
		{
			int pos = HEADER_SIZE;
			unsigned char Direct = *(unsigned char*)(message + pos);
			pos += sizeof(Direct);
			short X = *(short*)(message + pos);
			pos += sizeof(X);
			short Y = *(short*)(message + pos);

			session->_Player->_Direct = (Direction)Direct;

			Packet packet;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(unsigned char) + sizeof(short) + sizeof(short);
			header.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_002;

			packet.PutData((char*)&header, HEADER_SIZE);

			packet << session->ID;
			packet << Direct;
			packet << X;
			packet << Y;
			session->_Player->NotifyPlayer(&X, &Y, nullptr);
			SendBroadcast(session, packet.GetBufferPtr(), HEADER_SIZE + header.BySize);

			CheckDamage(session, ATTACK_TYPE::ATTACK002);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_003:
		{
			int pos = HEADER_SIZE;
			unsigned char Direct = *(unsigned char*)(message + pos);
			pos += sizeof(Direct);
			short X = *(short*)(message + pos);
			pos += sizeof(X);
			short Y = *(short*)(message + pos);

			session->_Player->_Direct = (Direction)Direct;

			Packet packet;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(unsigned char)  + sizeof(short) + sizeof(short);
			header.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_003;

			packet.PutData((char*)&header, HEADER_SIZE);

			packet << session->ID;
			packet << Direct;
			packet << X;
			packet << Y;
			session->_Player->NotifyPlayer(&X, &Y, nullptr);
			SendBroadcast(session, packet.GetBufferPtr(), HEADER_SIZE + header.BySize);

			CheckDamage(session, ATTACK_TYPE::ATTACK003);
		}
		break;
		default:
			Disconnect(session);
			break;
		}
	}
}

void FighterServer::WriteProc(Session* session)
{
	while (1)
	{
		if (session->SendBuffer.GetUseSize() <= 0)
			return;

		int retval;
		int sendAvailableSize = session->SendBuffer.DirectDequeueSize();
		char* ptr = session->SendBuffer.GetFrontBufferPtr();
		retval = send(session->Sock, ptr, sendAvailableSize, 0);
		session->SendBuffer.MoveFront(retval);

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
	}
}

void FighterServer::CheckDamage(Session* session, ATTACK_TYPE attackType)
{
	MyList<Session*>::iterator iter;
	for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
	{
		if (*iter == session)
			continue;

		bool result = session->_Player->OnAttack((*iter)->_Player, attackType);
		if (result)
		{
			Packet Damage;
			PACKET_HEADER header;
			header.ByCode = 0x89;
			header.BySize = sizeof(int) + sizeof(int) + sizeof(unsigned char);
			header.ByType = (unsigned char)PacketType::FIGHTER_CMD_DAMAGE;

			Damage.PutData((char*)&header, HEADER_SIZE);

			Damage << session->ID;
			Damage << (*iter)->ID;
			unsigned char DamageHP;
			(*iter)->_Player->NotifyPlayer(nullptr, nullptr, &DamageHP);
			Damage << DamageHP;
			SendBroadcast(nullptr, Damage.GetBufferPtr(), HEADER_SIZE + header.BySize);

			// 어택 로그
			wprintf(L"[Attack] Attack ID : %d, Attacked ID : %d, AttackType : %d \n",
				session->ID, (*iter)->ID, attackType);

			if ((*iter)->_Player->IsDead())
			{
				Packet Delete;
				header.BySize = sizeof(int);

				Delete.PutData((char*)&header, HEADER_SIZE);

				Damage << (*iter)->ID;
				SendBroadcast(nullptr, Delete.GetBufferPtr(), HEADER_SIZE + header.BySize);

				// 죽었으니 연결을 끊어줘야 한다.
				Disconnect(*iter);
			}
		}
	}
}

void FighterServer::SendUnicast(Session* session, char* message, int size)
{
	if (session->SendBuffer.GetFreeSize() > size)
		session->SendBuffer.Enqueue(message, size);
	else
		Disconnect(session);
}

void FighterServer::SendBroadcast(Session* session, char* message, int size)
{
	MyList<Session*>::iterator iter;
	for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
	{
		if (*iter == session)
			continue;

		if ((*iter)->SendBuffer.GetFreeSize() > size)
			(*iter)->SendBuffer.Enqueue(message, size);
		else
			Disconnect(*iter);
	}
}

void FighterServer::Disconnect(Session* session)
{
	// 1. Delete 패킷 전송
	Packet Delete;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = sizeof(int);
	header.ByType = (unsigned char)PacketType::FIGHTER_CMD_DELETE_CHARACTER;

	Delete.PutData((char*)&header, HEADER_SIZE);
	Delete << session->ID;

	SendBroadcast(session, Delete.GetBufferPtr(), HEADER_SIZE+header.BySize);

	// Session 제거 알려주기
	wprintf(L"[Client Disconnect] ID : %d, IP : %s, Port : %d\n",
		session->ID, session->IP, session->Port);

	// 2. ClientList 에서 삭제 => 우선 임시로 삭제 리스트에 추가
	deleteClients.push_back(session);
}

void FighterServer::DeleteSessions()
{
	MyList<Session*>::iterator iter;
	for (iter = deleteClients.begin(); iter != deleteClients.end(); ++iter)
	{
		clientSocks.remove(*iter);
		closesocket((*iter)->Sock);
		delete ((*iter)->_Player);
		delete (*iter);
	}
	deleteClients.clear();
}

void FighterServer::Close() 
{
	closesocket(listenSock);
}