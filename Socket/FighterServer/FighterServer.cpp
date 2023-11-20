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

		// 프레임 계산 필요.

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
	
	Player* player = new Player;
	session->_Player = player;

	// 1. 당사자에게 생성됐음을 알려주기
	FIGHTER_CMD_CREATE_MY_CHARACTER packet;
	packet.ByCode = 0x89;
	packet.BySize = sizeof(FIGHTER_CMD_CREATE_MY_CHARACTER);
	packet.ByType = (unsigned char)PacketType::FIGHTER_CMD_CREATE_MY_CHARACTER;
	packet.Direct = Direction::RR;
	packet.ID = UniqueID;
	player->NotifyPlayer(&packet.X, &packet.Y, &packet.HP);
	SendUnicast(session, (char*)&packet, sizeof(PACKET_HEADER)+packet.BySize);

	clientSocks.push_back(session);

	// 2. 다른 사람에게 알려주기
	FIGHTER_CMD_CREATE_OTHER_CHARACTER otherPacket;
	otherPacket.ByCode = 0x89;
	otherPacket.BySize = sizeof(FIGHTER_CMD_CREATE_OTHER_CHARACTER);
	otherPacket.ByType = (unsigned char)PacketType::FIGHTER_CMD_CREATE_OTHER_CHARACTER;
	otherPacket.Direct = Direction::RR;
	otherPacket.ID = UniqueID;
	player->NotifyPlayer(&otherPacket.X, &otherPacket.Y, &otherPacket.HP);
	SendBroadcast(session, (char*)&otherPacket, sizeof(PACKET_HEADER)+otherPacket.BySize);

	// 3. 다른 사람 위치 받기
	MyList<Session*>::iterator iter;
	for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
	{
		if (*iter == session)
			continue;

		otherPacket.ID = (*iter)->ID;
		(*iter)->_Player->NotifyPlayer(&otherPacket.X, &otherPacket.Y, &otherPacket.HP);

		if (session->SendBuffer.GetFreeSize() > sizeof(PACKET_HEADER)+otherPacket.BySize)
			session->SendBuffer.Enqueue((char*)&otherPacket, sizeof(PACKET_HEADER)+otherPacket.BySize);
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
		PACKET_HEADER header;
		session->RecvBuffer.Peek((char*)&header, sizeof(PACKET_HEADER));
		if (session->RecvBuffer.GetUseSize() < sizeof(PACKET_HEADER) + header.BySize)
			break;

		// 프로토콜 코드가 맞지 않다면 내보낸다.
		if (header.ByCode != 0x89)
			break;

		char message[MAX_PACKET_SIZE];
		retval = session->RecvBuffer.Dequeue(message, sizeof(PACKET_HEADER)+header.BySize);

		PacketType type = (PacketType)(((PACKET_HEADER*)message)->ByType);
		switch (type)
		{
		case PacketType::FIGHTER_QRY_MOVE_START:
		{
			FIGHTER_QRY_MOVE_START packet = *((FIGHTER_QRY_MOVE_START*)message);
			session->_Player->_MoveType = packet.Move;
			session->_Player->MovePos(packet.X, packet.Y, true);

			FIGHTER_REP_MOVE_START response;
			response.ByCode = 0x89;
			response.BySize = sizeof(FIGHTER_REP_MOVE_START);
			response.ByType = (unsigned char)PacketType::FIGHTER_REP_MOVE_START;
			response.Move = session->_Player->_MoveType;
			response.ID = session->ID;
			response.X = packet.X;
			response.Y = packet.Y;

			// 이 유저가 움직이기 시작했다는 것을 모두에게 알려줘야 한다.
			SendBroadcast(session, (char*)&response, sizeof(PACKET_HEADER)+response.BySize);
		}
		break;
		case PacketType::FIGHTER_QRY_MOVE_STOP:
		{
			FIGHTER_QRY_MOVE_STOP packet = *((FIGHTER_QRY_MOVE_STOP*)message);
			session->_Player->_Direct = packet.Direct;
			session->_Player->MovePos(packet.X, packet.Y, false);

			FIGHTER_REP_MOVE_STOP response;
			response.ByCode = 0x89;
			response.BySize = sizeof(FIGHTER_REP_MOVE_STOP);
			response.ByType = (unsigned char)PacketType::FIGHTER_REP_MOVE_STOP;
			response.Direct = session->_Player->_Direct;
			response.ID = session->ID;
			response.X = packet.X;
			response.Y = packet.Y;
			SendBroadcast(session, (char*)&response, sizeof(PACKET_HEADER) + response.BySize);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_001:
		{
			FIGHTER_QRY_ATTACK_001 packet = *((FIGHTER_QRY_ATTACK_001*)message);
			session->_Player->_Direct = packet.Direct;

			FIGHTER_REP_ATTACK_001 response;
			response.ByCode = 0x89;
			response.BySize = sizeof(FIGHTER_REP_ATTACK_001);
			response.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_001;
			response.Direct = session->_Player->_Direct;
			response.ID = session->ID;
			session->_Player->NotifyPlayer(&response.X, &response.Y, nullptr);
			SendBroadcast(session, (char*)&response, sizeof(PACKET_HEADER) + response.BySize);

			// 충돌처리
			CheckDamage(session, ATTACK_TYPE::ATTACK001);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_002:
		{
			FIGHTER_QRY_ATTACK_002 packet = *((FIGHTER_QRY_ATTACK_002*)message);
			session->_Player->_Direct = packet.Direct;

			FIGHTER_REP_ATTACK_002 response;
			response.ByCode = 0x89;
			response.BySize = sizeof(FIGHTER_REP_ATTACK_002);
			response.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_002;
			response.Direct = session->_Player->_Direct;
			response.ID = session->ID;
			session->_Player->NotifyPlayer(&response.X, &response.Y, nullptr);
			SendBroadcast(session, (char*)&response, sizeof(PACKET_HEADER) + response.BySize);
			
			CheckDamage(session, ATTACK_TYPE::ATTACK002);
		}
		break;
		case PacketType::FIGHTER_QRY_ATTACK_003:
		{
			FIGHTER_QRY_ATTACK_003 packet = *((FIGHTER_QRY_ATTACK_003*)message);
			session->_Player->_Direct = packet.Direct;

			FIGHTER_REP_ATTACK_003 response;
			response.ByCode = 0x89;
			response.BySize = sizeof(FIGHTER_REP_ATTACK_003);
			response.ByType = (unsigned char)PacketType::FIGHTER_REP_ATTACK_003;
			response.Direct = session->_Player->_Direct;
			response.ID = session->ID;
			session->_Player->NotifyPlayer(&response.X, &response.Y, nullptr);
			SendBroadcast(session, (char*)&response, sizeof(PACKET_HEADER) + response.BySize);
		
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
		char* ptr = session->RecvBuffer.GetFrontBufferPtr();
		retval = send(session->Sock, ptr, sendAvailableSize, 0);
		session->RecvBuffer.MoveFront(retval);

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
	FIGHTER_CMD_DAMAGE damage;
	damage.ByCode = 0x89;
	damage.BySize = sizeof(FIGHTER_CMD_DAMAGE);
	damage.ByType = (unsigned char)PacketType::FIGHTER_CMD_DAMAGE;
	damage.AttackID = session->ID;

	MyList<Session*>::iterator iter;
	for (iter = clientSocks.begin(); iter != clientSocks.end(); ++iter)
	{
		if (*iter == session)
			continue;

		bool result = (*iter)->_Player->OnAttackRange(session->_Player, attackType);
		if (result)
		{
			damage.DamageID = (*iter)->ID;
			(*iter)->_Player->NotifyPlayer(nullptr, nullptr, &damage.DamageHP);
			SendBroadcast(nullptr, (char*)&damage, sizeof(PACKET_HEADER) + damage.BySize);

			if ((*iter)->_Player->IsDead())
			{
				FIGHTER_CMD_DELETE_CHARACTER del;
				del.ByCode = 0x89;
				del.BySize = sizeof(FIGHTER_CMD_DELETE_CHARACTER);
				del.ByType = (unsigned char)PacketType::FIGHTER_CMD_DELETE_CHARACTER;
				del.ID = (*iter)->ID;
				SendBroadcast(nullptr, (char*)&del, sizeof(PACKET_HEADER) + del.BySize);
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
	FIGHTER_CMD_DELETE_CHARACTER packet;
	packet.ByCode = 0x89;
	packet.BySize = sizeof(FIGHTER_CMD_DELETE_CHARACTER);
	packet.ByType = (unsigned char)PacketType::FIGHTER_CMD_DELETE_CHARACTER;
	packet.ID = session->ID;
	SendBroadcast(session, (char*)&packet, packet.BySize);

	// 2. ClientList 에서 삭제 => 우선 임시로 삭제 리스트에 추가
	deleteClients.push_back(session);
}

void FighterServer::Close() 
{
	closesocket(listenSock);
}