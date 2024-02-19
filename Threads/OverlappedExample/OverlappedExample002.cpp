#include "OverlappedExample002.h"

SOCKET clientSock;
HANDLE hReadEvent, hWriteEvent;

int OverlapMain002()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
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

	
	// 이벤트 객체 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL)
		return 1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
		return 1;
	
	// 스레드 생성
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread002, nullptr, 0, nullptr);
	if (hThread == NULL)
		return 1;
	CloseHandle(hThread);

	while (1)
	{
		WaitForSingleObject(hReadEvent, INFINITE);
		// accept()
		clientSock = accept(listenSock, NULL, NULL);
		if (clientSock == INVALID_SOCKET)
			break;
		SetEvent(hWriteEvent);
	}

	// 윈속 종료
	WSACleanup();

	return 0;
}

unsigned int WINAPI WorkerThread002(LPVOID arg)
{
	int retval;
	WCHAR IP[16];

	while (1)
	{
		while (1)
		{
			// alertable wait
			DWORD result = WaitForSingleObjectEx(hWriteEvent, INFINITE, TRUE);
			if (result == WAIT_OBJECT_0)
				break;
			if (result != WAIT_IO_COMPLETION)
				return 1;
		}

		// 접속한 클라이언트 정보 출력
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(clientSock, (SOCKADDR*)&clientAddr, &addrLen);
		InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
		wprintf(L"\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n",
			IP, ntohs(clientAddr.sin_port));

		// 소켓 정보 구조체 할당과 초기화
		SOCKETINFO* ptr = new SOCKETINFO;
		if (ptr == nullptr)
			return 1;

		memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
		ptr->sock = clientSock;
		SetEvent(hReadEvent);
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		// 비동기 입출력 시작
		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, CompletionRoutine);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				return 1;
		}
	}
}

void CALLBACK CompletionRoutine(
	DWORD dwError, DWORD Transferred,
	LPWSAOVERLAPPED lpOverlapped, DWORD Flags
)
{
	int retval;
	WCHAR IP[16];

	// 클라이언트 정보 얻기
	SOCKETINFO* ptr = (SOCKETINFO*)lpOverlapped;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

	// 비동기 입출력 결과 확인
	if (dwError != 0 || Transferred == 0)
	{
		if (dwError != 0)
		{
			printf("Completion Routine Error!\n");
			return;
		}
		closesocket(ptr->sock);
		InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
		wprintf(L"[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n",
			IP, ntohs(clientAddr.sin_port));
		delete ptr;
		return;
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
		ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
		ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

		DWORD sendbytes;
		retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes,
			0, &ptr->overlapped, CompletionRoutine);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				return;
		}
	}
	else
	{
		ptr->recvbytes = 0;

		// 데이터 받기
		memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, CompletionRoutine);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				return;
		}
	}
}