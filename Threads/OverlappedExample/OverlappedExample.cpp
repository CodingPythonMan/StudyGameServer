#include "OverlappedExample.h"

int TotalSockets = 0;
SOCKETINFO* SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION cs;

int OverlapMain()
{
	int retval;
	InitializeCriticalSection(&cs);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return 1;

	// bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 1;

	// listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return 1;

	// 더미 이벤트 객체 생성
	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT)
		return 1;

	EventArray[TotalSockets++] = hEvent;

	// 스레드 생성
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	if (hThread == NULL)
		return 1;
	CloseHandle(hThread);

	// 데이터 통신에 사용할 변수
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	DWORD recvbytes, flags;
	WCHAR IP[16];

	while (1)
	{
		// accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
			break;

		InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
		wprintf(L"\n[TCP Server] Client Connect : IP Address=%s, Port=%d\n",
			IP, ntohs(clientAddr.sin_port));

		// 소켓 정보 추가
		if (AddSocketInfo(clientSock) == FALSE)
		{
			closesocket(clientSock);
			InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
			wprintf(L"[TCP Server] Client Terminate : IP Address=%s, Port=%d\n",
				IP, ntohs(clientAddr.sin_port));
			continue;
		}

		// 비동기 입출력 시작
		SOCKETINFO* ptr = SocketInfoArray[TotalSockets - 1];
		flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				RemoveSocketInfo(TotalSockets - 1);
				continue;
			}
		}

		// 소켓의 개수 변화를 알림
		WSASetEvent(EventArray[0]);
	}

	// 윈속 종료
	WSACleanup();
	DeleteCriticalSection(&cs);

	return 0;
}

// 비동기 입출력 처리 함수
unsigned int __stdcall WorkerThread(LPVOID arg)
{
	int retval;
	WCHAR IP[16];

	while (1)
	{
		// 이벤트 객체 관찰
		int index = WSAWaitForMultipleEvents(TotalSockets,
			EventArray, FALSE, WSA_INFINITE, FALSE);

		if (index == WSA_WAIT_FAILED)
			continue;

		index -= WSA_WAIT_EVENT_0;
		WSAResetEvent(EventArray[index]);
		if (index == 0)
			continue;

		// 클라이언트 정보 얻기
		SOCKETINFO* ptr = SocketInfoArray[index];
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

		// 비동기 입출력 결과 확인
		DWORD Transferred, flags;
		retval = WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
			&Transferred, FALSE, &flags);

		if (retval == FALSE || Transferred == 0)
		{
			RemoveSocketInfo(index);
			InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
			wprintf(L"[TCP Server] Client Terminate : IP Address=%s, Port=%d\n",
				IP, ntohs(clientAddr.sin_port));
			
			continue;
		}

		// 데이터 전송량 갱신
		if (ptr->recvbytes == 0)
		{
			ptr->recvbytes = Transferred;
			ptr->sendbytes = 0;
			// 받은 데이터 출력
			ptr->buf[ptr->recvbytes] = '\0';
			InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
			wprintf(L"[TCP/%s:%d] ", IP, ntohs(clientAddr.sin_port));
			printf("%s\n", ptr->buf);
		}
		else
		{
			ptr->sendbytes += Transferred;
		}

		if (ptr->recvbytes > ptr->sendbytes)
		{
			// 데이터 보내기
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					return 1;

				continue;
			}
		}
		else
		{
			ptr->recvbytes = 0;

			// 데이터 받기
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			flags = 0;
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					return 1;

				continue;
			}
		}
	}

	return 0;
}

BOOL AddSocketInfo(SOCKET sock)
{
	EnterCriticalSection(&cs);
	if (TotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)
		return FALSE;

	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == nullptr)
		return FALSE;

	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT)
		return FALSE;

	memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
	ptr->overlapped.hEvent = hEvent;
	ptr->sock = sock;
	ptr->recvbytes = ptr->sendbytes = 0;
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = BUFSIZE;
	SocketInfoArray[TotalSockets] = ptr;
	EventArray[TotalSockets] = hEvent;
	TotalSockets++;

	LeaveCriticalSection(&cs);

	return TRUE;
}

void RemoveSocketInfo(int index)
{
	EnterCriticalSection(&cs);

	SOCKETINFO* ptr = SocketInfoArray[index];
	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[index]);

	if (index != (TotalSockets - 1))
	{
		SocketInfoArray[index] = SocketInfoArray[TotalSockets - 1];
		EventArray[index] = EventArray[TotalSockets - 1];
	}

	TotalSockets--;

	LeaveCriticalSection(&cs);
}