// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab9.h"
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
	LoadStringW(hInstance, IDC_LAB9, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB9));

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
	wcex.hIcon = LoadIcon(hInstance,
		MAKEINTRESOURCE(IDC_LAB9));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB9);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,
		MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	static bool lclicked = FALSE;

	static int mouse_x = NO_POINT;
	static int mouse_y = NO_POINT;

	static int secatel_x = NO_POINT;
	static int secatel_y = NO_POINT;

	static int x = -1, y = -1;

	static Table* cuts = NULL;
	static Table* secatel = NULL;

	switch (message)
	{
	case WM_CREATE:
	{
		creating(hWnd);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (!lclicked)
			break;
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (LOWORD(lParam) > rc.right - 300 || HIWORD(lParam) > rc.bottom)
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
		}
		getMouseCoordinates(lParam,
			rc.right - 300, x, y);
		if (IsDlgButtonChecked(hWnd, ID_RBUTTON_SECATEL))
		{
			secatel = addToTable(secatel, newPoint(x, y));
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			secatel = deleteLast(secatel);
		}
		else
		{
			if (IsDlgButtonChecked(hWnd, ID_RBUTTON_VERTICAL))
			{
				if (abs(x - mouse_x) > abs(y - mouse_y))
					y = mouse_y;
				else
					x = mouse_x;
			}
			cuts = addToTable(cuts, newPoint(x, y));
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			cuts = deleteLast(cuts);
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		lclicked = true;
		getMouseCoordinates(lParam,
			rc.right - 300, x, y);
		if (x > rc.right - 300 || y > rc.bottom)
			break;
		if (IsDlgButtonChecked(hWnd, ID_RBUTTON_SECATEL))
		{
			if (secatel_x != NO_POINT)
			{
				secatel = addToTable(secatel,
					newPoint(x, y));
			}
			secatel_x = x;
			secatel_y = y;
		}
		else
		{
			if (mouse_x != NO_POINT)
			{
				if (IsDlgButtonChecked(hWnd, ID_RBUTTON_VERTICAL))
				{
					if (abs(x - mouse_x) > abs(y - mouse_y))
						y = mouse_y;
					else
						x = mouse_x;
				}
				cuts = addToTable(cuts,
					newPoint(x, y));
			}
			mouse_x = x;
			mouse_y = y;
		}
		cleanRectOld(hWnd, 0, 0, 300, 0);
		drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
		break;
	}
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		cleanRectOld(hWnd, 0, 0, 300, 0);
		drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
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
				listview_y2, cuts, secatel);
		}
		}
	}
	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_RBUTTON_SECATEL:
		{
			secatel = unlockTable(secatel);
			lclicked = false;
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 180));
			break;
		}
		case ID_RBUTTON_VERTICAL:
		{
			;
		}
		case ID_RBUTTON_CUTS:
		{
			cuts = unlockTable(cuts);
			lclicked = false;
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 180));
			break;
		}
		case ID_BUTTON_BUILD:
		{
			lclicked = false;

			secatel = lockTable(secatel);
			cuts = lockTable(cuts);
			
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(220, 220, 220));
			HDC hdc = GetDC(hWnd);
			int n = drawTableInside(hdc, cuts, secatel, RGB(50, 50, 50));

			ReleaseDC(hWnd, hdc);

			switch (n)
			{
			case SECATEL_NO:
			{
				MessageBox(hWnd, "Секатель не введён",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case SECATEL_IS_ONE_CUT:
			{
				MessageBox(hWnd,
					"Секатель должен состоять более, чем из одного отрезка",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case SECATEL_IS_ONE_POINT:
			{
				MessageBox(hWnd,
					"Секатель должен состоять более, чем из одной точки",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case SECATEL_ISNT_CONVEX:
			{
				MessageBox(hWnd,
					"Секатель невыпуклый. Пожалуйста введите секатель заново.",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case NO_CUTS:
			{
				MessageBox(hWnd, "Не указаны отрезки.",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case NO_CUTS_AND_SECATEL:
			{
				MessageBox(hWnd, "Не указаны отрезки.",
					"Некорректные входные данные", MB_OK);
				break;
			}
			}
			break;
		}
		case ID_BUTTON_LOCK:
		{
			secatel = lockTable(secatel);
			cuts = lockTable(cuts);
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			break;
		}
		case ID_BUTTON_DELETE:
		{
			if (getTableSize(cuts) == 0)
				MessageBox(NULL,
					"Ещё не введено ни одного отрезка, ничего нельзя удалить",
					"Ошибка", MB_OK);
			else
			{
				deleting(hWnd, listview_choosen,
					listview_x1, listview_y1,
					listview_x2, listview_y2, cuts);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			}
			break;
		}
		case ID_BUTTON_CHANGE:
		{
			if (getTableSize(cuts) == 0)
				MessageBox(NULL,
					"Ещё не введено ни одного отрезка, ничего нельзя удалить",
					"Ошибка", MB_OK);
			else
			{
				changing(hWnd, listview_choosen, cuts,
					listview_x1, listview_y1,
					listview_x2, listview_y2);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			}
			break;
		}
		case ID_BUTTON_CLEAR:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			deleteTable(&cuts);
			deleteTable(&secatel);
			secatel_x = NO_POINT;
			secatel_y = NO_POINT;
			ListView_DeleteAllItems(GetDlgItem(hWnd, ID_LISTVIEW));
			break;
		}
		case ID_BUTTON_ADD:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			cuts = adding(hWnd, cuts);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
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
INT_PTR CALLBACK About(HWND hDlg,
	UINT message, WPARAM wParam, LPARAM lParam)
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
