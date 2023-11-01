// Drawer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Drawer.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HBRUSH gTileBrush;
HPEN gGridPen;
bool gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음

// 타일의 속성 입력 / 제거 모드 플래그
// 더블 클릭 시 해당 타일의 속성을 단순 반전만 시켜준다면 드래그를 통해서 장애물 입력을 수월하게 하기가 어렵다.
// 첫 클릭 시 해당 타일의 속성에 따라서 장애물 입력모드/제거모드를 본 플래그로 지정하여, 마우스 이동시 지정된 방법으로
// 장애물을 연속적으로 셋팅하도록 한다.

bool gErase = false;
bool gDrag = false;

void RenderGrid(HDC hdc)
{
    int X = 0;
    int Y = 0;
    HPEN OldPen = (HPEN)SelectObject(hdc, gGridPen);
    // 그리드의 마지막 선을 추가로 그리기 위해 <= 의 반복 조건
    for (int i = 0; i <= GRID_WIDTH; i++)
    {
        MoveToEx(hdc, X, 0, NULL);
        LineTo(hdc, X, GRID_HEIGHT * GRID_SIZE);
        X += GRID_SIZE;
    }

    for (int i = 0; i <= GRID_HEIGHT; i++)
    {
        MoveToEx(hdc, 0, Y, NULL);
        LineTo(hdc, GRID_WIDTH * GRID_SIZE, Y);
        Y += GRID_SIZE;
    }
    SelectObject(hdc, OldPen);
}

void RenderObstacle(HDC hdc)
{
    int X = 0;
    int Y = 0;
    HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gTileBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    // 사각형의 테두리를 안보이도록 하기위해 Null Pen 을 지정한다.
    // CreatePen 으로 Null Pen을 생성해도 되지만, GetStockObject 를 사용하여
    // 이미 시스템에 만들어져 있는 고정 GDI Object 를 사용해본다.
    // GetStockObject 는 시스템의 고정적인 범용 GDI Object 로서 삭제가 필요 없다.
    // 시스템 전역적인 GDI Object 를 얻어서 사용한다는 개념.
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            if (gTile[j][i])
            {
                X = i * GRID_SIZE;
                Y = j * GRID_SIZE;
                // 테두리 크기가 있으므로 + 2 한다.
                Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
            }
        }
    }
    SelectObject(hdc, OldBrush);
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
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
    LoadStringW(hInstance, IDC_DRAWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWER));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRAWER);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL g_Click; // 마우스 클릭여부 판단
HPEN g_Pen; // 현재 생성된 펜, 휠버튼 클릭 시 새롭게 생성
int g_OldX; // 마우스 드래그 표현을 위한 마우스 이동시 이전 좌표
int g_OldY; // 마우스 드래그 표현을 위한 마우스 이동시 이전 좌표
// Liner
LRESULT CALLBACK WndProc_Liner(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_Pen = CreatePen(PS_SOLID, rand() % 20, RGB(rand() % 255, rand() % 255, rand() % 255));
        break;
    case WM_MBUTTONDOWN:
        if (g_Pen != NULL)
            DeleteObject(g_Pen);
        g_Pen = CreatePen(PS_SOLID, rand() % 20, RGB(rand() % 255, rand() % 255, rand() % 255));
        break;
    case WM_LBUTTONDOWN:
        g_Click = true;
        break;
    case WM_LBUTTONUP:
        g_Click = false;
        break;
    case WM_MOUSEMOVE:
    {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (g_Click)
		{
			HDC hdc = GetDC(hWnd);
			HPEN hPenOld = (HPEN)SelectObject(hdc, g_Pen);
			MoveToEx(hdc, g_OldX, g_OldY, NULL);
			LineTo(hdc, xPos, yPos);
			SelectObject(hdc, hPenOld);
			ReleaseDC(hWnd, hdc);
		}
		break;
    }
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(g_Pen);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc_Dotter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
    case WM_CREATE:
        SetTimer(hWnd, 1, 1, NULL);
        SetTimer(hWnd, 2, 2, NULL);
        break;
    case WM_TIMER:
    {
		HDC hdc;
		hdc = GetDC(hWnd);
        switch (wParam)
        {
        case 1:
            SetPixel(hdc, rand() % 500, rand() % 500, RGB(255,0,0));
            break;
        case 2:
            SetPixel(hdc, rand() % 500, rand() % 500, RGB(0, 0, 255));
            break;
        }
        ReleaseDC(hWnd, hdc);
        break;
    }
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
	switch (message)
	{
    case WM_LBUTTONDOWN:
        gDrag = true;
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            int TileX = xPos / GRID_SIZE;
            int TileY = yPos / GRID_SIZE;
            // 첫 선택 타일이 장애물이면 지우기 모드 아니면 장애물 넣기 모드
            if (gTile[TileY][TileX])
                gErase = true;
            else
                gErase = false;
        }
        break;
    case WM_LBUTTONUP:
        gDrag = false;
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
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
