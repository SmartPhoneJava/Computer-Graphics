// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab8.h"
#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>

using namespace std;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int listview_choosen = 0;
int listview_x1 = 0;
int listview_y1 = 0;
int listview_x2 = 0;
int listview_y2 = 0;

// Отправить объявления функций, включенных в этот модуль кода:
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

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LAB8));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB8);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	static bool lclicked = FALSE;
	static bool rclicked = FALSE;
	static int catched = 0;
	static bool ctrl = FALSE;

	static Table* table = NULL;

	static int lx = NO_POINT, ly = NO_POINT;
	static int rx = NO_POINT, ry = NO_POINT;
	static int count = 0;
	static Secatel *secatel = newSecatel(100,
		400, 200, 100);

	switch (message)
	{
	case WM_KEYDOWN: //контрл
	{
		if (wParam == 17)
		{
			;
		}
		else if (wParam == 16) //шифт
		{
			;
		}
		break;
	}
	case WM_KEYUP:
	{
		break;
	}
	case WM_CREATE:
	{
		creating(hWnd);
		break;
	}
	case WM_RBUTTONUP:
	{
		rclicked = FALSE;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		rclicked = TRUE;
		break;
	}
	case WM_LBUTTONUP:
	{
		rclicked = FALSE;
		catched = 0;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		rclicked = TRUE;
		lclicked = !lclicked;
		if (lclicked)
			mouseClicked(lParam, 700, lx, ly,
				hWnd, ID_EDIT_X1, ID_EDIT_Y1);
		else
		{
			mouseClicked(lParam, 700, rx, ry,
				hWnd, ID_EDIT_X2, ID_EDIT_Y2);

			cleanRectOld(hWnd, 0, 0, 300, 0);
			table = adding(hWnd, table);
			drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (rclicked)
		{
			lclicked = FALSE;
			secatelControl(lParam,
				*secatel, catched, 700);
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
		}
		break;
	}
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		cleanRectOld(hWnd, 0, 0, 300, 0);
		drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_SIZE:
	{
		moving(hWnd, false);
		break;
	}
	case WM_NOTIFY:
	{
		HWND edit_x = GetDlgItem(hWnd, ID_EDIT_X1);
		HWND edit_y = GetDlgItem(hWnd, ID_EDIT_Y1);
		HWND nedit_x = GetDlgItem(hWnd, ID_NEDIT_X1);
		HWND nedit_y = GetDlgItem(hWnd, ID_NEDIT_Y1);
		HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
		{
			updatingListView(hWnd, lParam,
				listview_choosen, listview_x1,
				listview_y1, listview_x2,
				listview_y2, table, secatel);
		}
		}
	}
	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_BUTTON_BUILD:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, table, secatel, RGB(180, 180, 180));

			HDC hdc = GetDC(hWnd);
			drawTableInside(hdc, table, *secatel, RGB(0, 0, 255));

			//drawFigure(hWnd);
			ReleaseDC(hWnd, hdc);
			break;
		}
		case ID_BUTTON_DELETE:
		{
			if (getTableSize(table) == 0)
				MessageBox(NULL, "There is no any point. You cant delete anything!",
					"Warning", MB_OK);
			else
			{
				deleting(hWnd, listview_choosen,
					listview_x1, listview_y1,
					listview_x2, listview_y2, table);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
			}
			break;
		}
		case ID_BUTTON_CHANGE:
		{
			if (getTableSize(table) == 0)
				MessageBox(NULL, "There is no any point. You cant change anything!",
					"Warning", MB_OK);
			else
			{
				changing(hWnd, listview_choosen, table,
					listview_x1, listview_y1,
					listview_x2, listview_y2);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
			}
			break;
		}
		case ID_BUTTON_CLEAR:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			deleteTable(&table);
			ListView_DeleteAllItems(GetDlgItem(hWnd, ID_LISTVIEW));
			break;
		}
		case ID_BUTTON_ADD:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			table = adding(hWnd, table);
			drawPicture(hWnd, table, secatel, RGB(0, 0, 150));
			break;
		}
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		SetFocus(hWnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	SetFocus(hWnd);
	return 0;
}

// Обработчик сообщений для окна "О программе".
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
