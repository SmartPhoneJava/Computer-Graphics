// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab10.h"
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
	LoadStringW(hInstance, IDC_LAB10, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LAB10));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB10);
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

	static int mouse_x = NO_POINT;
	static int mouse_y = NO_POINT;

	static int secatel_x = NO_POINT;
	static int secatel_y = NO_POINT;
	static int count = 0;

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
		if (LOWORD(lParam) > 800 || HIWORD(lParam) > 800)
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
		}
		getMouseCoordinates(lParam,
			800, x, y);
		if (IsDlgButtonChecked(hWnd, ID_RBUTTON_SECATEL))
		{
			if (secatel_x != NO_POINT)
			{
				Cut *cut = new Cut(secatel_x, secatel_y, 0, x, y, 0);
				secatel = addToTable(secatel, cut);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
				secatel = deleteOfTable(secatel, cut);
			}
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
				Cut *cut = new Cut(mouse_x, mouse_y, 0, x, y, 0);
				cuts = addToTable(cuts, cut);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
				cuts = deleteOfTable(cuts, cut);
			}
		}
		
		break;
	}
	case WM_LBUTTONDOWN:
	{
		/*
		getMouseCoordinates(lParam,
			800, x, y);
		if (x > 800 || y > 800)
			break;
		lclicked = true;
		if (IsDlgButtonChecked(hWnd, ID_RBUTTON_SECATEL))
		{
			if (secatel_x != NO_POINT)
			{
				secatel = addToTable(secatel,
					new Cut(secatel_x, secatel_y, 0, x, y, 0));
			}
			secatel_x = x;
			secatel_y = y;

			count = 0;
		}
		else
		{
			count++;
			if (count % 2) // остаток 1
			{
				setMouseCoordinates(hWnd, x, y,
					ID_EDIT_X1, ID_EDIT_Y1);
				mouse_y = y;
				mouse_x = x;
			}
			else
			{
				if (IsDlgButtonChecked(hWnd, ID_RBUTTON_VERTICAL))
				{
					if (mouse_x != NO_POINT)
					{
						if (abs(x - mouse_x) > abs(y - mouse_y))
							y = mouse_y;
						else
							x = mouse_x;
					}
				}
				setMouseCoordinates(hWnd, x, y,
					ID_EDIT_X2, ID_EDIT_Y2);
				cuts = adding(hWnd, cuts);
				mouse_y = NO_POINT;
				mouse_x = NO_POINT;
			}
		}
		cleanRectOld(hWnd, 0, 0, 300, 0);
		drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
		*/
		break;
	}
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		//cleanRectOld(hWnd, 0, 0, 300, 0);
		//drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
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
		/*
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
		*/
		break;
	}
	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_RBUTTON_VERTICAL:
		{
			;
		}
		case ID_RBUTTON_SECATEL:
		{
			;
		}
		case ID_RBUTTON_CUTS:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 180));
			break;
		}
		case ID_BUTTON_BUILD:
		{
			int xminEdit = 100;
			int xmaxEdit = 200;
			int dxEdit = 1;
			int nEdit = 20;
			int zminEdit = 100;
			int zmaxEdit = 200;

			CFunction function{};

			//f, xmin, xmax, dx, n, zmin, zmax
			tFunction func1{ function.GetFunc(2), xminEdit,
				xmaxEdit, dxEdit,
				nEdit, zminEdit, zmaxEdit};
			SimpleAlgo(hWnd, func1, &cuts);
			/*
			int flag = false;
			lclicked = false;

			if (!isLock(secatel))
			{
				secatel = lockTable(secatel);
				flag = true;
			}
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(220, 220, 220));
			HDC hdc = GetDC(hWnd);
			int n = drawTableInside(hdc, cuts, secatel, RGB(50, 50, 50));
			if (flag)
				secatel = unlockTable(secatel);

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
				MessageBox(hWnd, "Секатель должен состоять более, чем из одного отрезка",
					"Некорректные входные данные", MB_OK);
				break;
			}
			case SECATEL_ISNT_CONVEX:
			{
				MessageBox(hWnd, "Секатель невыпуклый. Пожалуйста введите секатель заново.",
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
			*/
			break;
		}
		case ID_BUTTON_LOCK:
		{
			/*
			TCHAR buffer[BUFFER_MAX_SIZE];
			GetWindowText(GetDlgItem(hWnd, ID_BUTTON_LOCK), buffer, sizeof(buffer));
			if (strcmp(buffer, "Замкнуть") == 0)
			{
				secatel = lockTable(secatel);
				setCharText(GetDlgItem(hWnd, ID_BUTTON_LOCK), "Разомкнуть");
			}
			else
			{
				secatel = unlockTable(secatel);
				setCharText(GetDlgItem(hWnd, ID_BUTTON_LOCK), "Замкнуть");
			}
			cleanRectOld(hWnd, 0, 0, 300, 0);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			*/
			break;
		}
		case ID_BUTTON_DELETE:
		{
			/*
			if (getTableSize(cuts) == 0)
				MessageBox(NULL, "Ещё не введено ни одного отрезка, ничего нельзя удалить",
					"Ошибка", MB_OK);
			else
			{
				deleting(hWnd, listview_choosen,
					listview_x1, listview_y1,
					listview_x2, listview_y2, cuts);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			}
			*/
			break;
		}
		case ID_BUTTON_CHANGE:
		{
			/*
			if (getTableSize(cuts) == 0)
				MessageBox(NULL, "Ещё не введено ни одного отрезка, ничего нельзя удалить",
					"Ошибка", MB_OK);
			else
			{
				changing(hWnd, listview_choosen, cuts,
					listview_x1, listview_y1,
					listview_x2, listview_y2);
				cleanRectOld(hWnd, 0, 0, 300, 0);
				drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			}
			*/
			break;
		}
		case ID_BUTTON_CLEAR:
		{
			/*
			cleanRectOld(hWnd, 0, 0, 300, 0);
			deleteTable(&cuts);
			deleteTable(&secatel);
			secatel_x = NO_POINT;
			secatel_y = NO_POINT;
			count = 0;
			ListView_DeleteAllItems(GetDlgItem(hWnd, ID_LISTVIEW));
			*/
			break;
		}
		/*
		case ID_BUTTON_ADD:
		{
			cleanRectOld(hWnd, 0, 0, 300, 0);
			cuts = adding(hWnd, cuts);
			drawPicture(hWnd, cuts, secatel, RGB(0, 0, 150));
			break;
		}
		*/
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
