// AsyncSelectClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AsyncSelectClient.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WS2tcpip.h>
#include <Windowsx.h>
#include "RingBuffer.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_LOADSTRING 100

#define SERVER_PORT 25000
#define SERVER_IP L"127.0.0.1"

#define WM_NETWORK (WM_USER+1)

bool ConnectSock = false;
bool Drag = false;
SOCKET Sock;

RingBuffer RecvBuffer(50);
RingBuffer SendBuffer(50);

// Render
HPEN Pen;
int OldX;
int OldY;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASYNCSELECTCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hwnd;
    hwnd = InitInstance(hInstance, nCmdShow);
    if (!hwnd)
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASYNCSELECTCLIENT));

    MSG msg;
    int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == INVALID_SOCKET)
		return 1;

	// WSAAsyncSelect()
	retval = WSAAsyncSelect(Sock, hwnd, WM_NETWORK, FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE);

	// Connect 하기 전에 WSAAsyncSelect 실행
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
    InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

	// Connect 통지는 WM Message 확인
    connect(Sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASYNCSELECTCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASYNCSELECTCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

bool WriteProc()
{
    int retval;
    int useSize = SendBuffer.GetUseSize();

	if (SendBuffer.DirectDequeueSize() < useSize)
	{
		char* buffer = new char[useSize];
		SendBuffer.Peek(buffer, useSize);
		retval = send(Sock, buffer, useSize, 0);
        delete[] buffer;
	}
	else
	{
		char* ptr = SendBuffer.GetFrontBufferPtr();
		retval = send(Sock, ptr, useSize, 0);
	}

    if (retval == SOCKET_ERROR)
    {
        retval = GetLastError();

        if (retval == WSAEWOULDBLOCK)
            return false;
		else if (retval == WSAECONNRESET)
		{
            closesocket(Sock);
			return true;
		}
    }

    SendBuffer.MoveFront(retval);

    return false;
}

bool ReadProc(HWND hWnd)
{
	// 데이터 받은 후 처리 필요
	int retval;

	int recvAvailableSize = RecvBuffer.DirectEnqueueSize();
    if (recvAvailableSize < 18)
    {
        char buffer[18];
        retval = recv(Sock, buffer, 18, 0);

        if (RecvBuffer.GetFreeSize() > 18)
            RecvBuffer.Enqueue(buffer, retval);
    }
    else
    {
		char* ptr = RecvBuffer.GetRearBufferPtr();
		retval = recv(Sock, ptr, recvAvailableSize, 0);
		RecvBuffer.MoveRear(retval);
    }

    // 받은 데이터를 기반으로 그려줘야한다.
    while (1)
    {
        if (RecvBuffer.GetUseSize() <= 2)
            break;

        stHEADER header;
        RecvBuffer.Peek((char*)&header, 2);

        if (RecvBuffer.GetUseSize() - 2 < header.Len)
            break;

        RecvBuffer.MoveFront(2);

        st_DRAW_PACKET payload;
        RecvBuffer.Dequeue((char*)&payload, header.Len);

		int xPos = payload.iEndX;
		int yPos = payload.iEndY;
        int xOld = payload.iStartX;
        int yOld = payload.iStartY;

		// Send 만 진행한다.
		HDC hdc = GetDC(hWnd);
		HPEN PenOld = (HPEN)SelectObject(hdc, Pen);
		MoveToEx(hdc, xOld, yOld, nullptr);
		LineTo(hdc, xPos, yPos);
		SelectObject(hdc, PenOld);
		ReleaseDC(hWnd, hdc);
    }

	return false;
}

bool NetworkMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 오류 발생 여부 확인
	if (WSAGETSELECTERROR(lParam))
	{
		closesocket(Sock);
		return true;
	}

    //FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE
    // 메세지 처리
    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT:
        ConnectSock = true;
        break;
    case FD_WRITE:
		if (ConnectSock && WriteProc())
		{
			return true;
		}
        break;
    case FD_READ:
        // 링버퍼에서 읽고 나서 한다.
        if (ConnectSock && ReadProc(hWnd))
        {
            return true;
        }
        break;
    case FD_CLOSE:
        closesocket(Sock);
        return true;
    default:
        closesocket(Sock);
        return true;
    }

    return false;
}

bool SendServer(char* message)
{
    stHEADER header;
    header.Len = sizeof(st_DRAW_PACKET);

    SendBuffer.Enqueue((char*)&header, 2);
    SendBuffer.Enqueue(message, header.Len);

    WriteProc();

    return false;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        Pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
        }
        break;
    case WM_NETWORK:
        if (NetworkMessage(hWnd, message, wParam, lParam))
        {
            PostQuitMessage(0);
        }
        break;
    case WM_LBUTTONDOWN:
		OldX = GET_X_LPARAM(lParam);
		OldY = GET_Y_LPARAM(lParam);
        Drag = true;
        break;
    case WM_LBUTTONUP:
        Drag = false;
        break;
    case WM_MOUSEMOVE:
        if (ConnectSock && Drag)
        {
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			// 링버퍼에 넣고 Send를 최선을 다해 진행
			st_DRAW_PACKET payLoad;
			payLoad.iStartX = OldX;
			payLoad.iStartY = OldY;
			payLoad.iEndX = xPos;
			payLoad.iEndY = yPos;

			if (SendServer((char*)&payLoad))
			{
				PostQuitMessage(0);
				break;
			}

			OldX = xPos;
			OldY = yPos;
        }
        break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
