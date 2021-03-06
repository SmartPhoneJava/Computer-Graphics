// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab2.h"
#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <CommCtrl.h>
#include <tchar.h>

using namespace std;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define ID_BUTTON_SCALE 1
#define ID_BUTTON_ROTATE 2
#define ID_BUTTON_MOVE 3
#define ID_BUTTON_FUTURE 4
#define ID_BUTTON_SAVE 5
#define ID_BUTTON_CANCEL 6
#define ID_BUTTON_CENTER 7
#define ID_BUTTON_BACK 8
#define ID_BUTTON_ORIGINAL 9

#define ID_EDIT_CDX 11 // Либо абсцисса центра, либо дельта X
#define ID_EDIT_CDY 12 // Либо ординат центра, либо дельта Y
#define ID_EDIT_KX 13 // Коэффициент масштабираования по X
#define ID_EDIT_KAY 14 // Либо угол, либо коэффициент масштабираования по Y

#define ID_TEXT_LABEL 30

#define ID_MODE_MENU 200
#define ID_MODE_MOVING 201
#define ID_MODE_SCALING 202
#define ID_MODE_ROTATING 203

#define BUFFER_MAX_SIZE 64

#define MAX_LOADSTRING 100

#define NO_ERROR 0
#define ERROR_NO_FIRST_TOP -1
#define ERROR_NO_SECOND_TOP -2
#define ERROR_NOT_ENOUGH_POINTS_FIRST -3
#define ERROR_NOT_ENOUGH_POINTS_SECOND -4

typedef struct Row
{
	int type;
	double dx;
	double dy;
	double cx;
	double cy;
	double kx;
	double ky;
	double angle;
	struct Row* next;
} Row;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int listview_choosen = 0;

int page = ID_MODE_MENU;

static double XStartPos = 100, YStartPos = 20, WidthGrid = 400, HeightGrid = 400, StepGrid = 2, Radius = 4;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Row* listRow;

Row* init_row();

Row* add_to_list(Row *list, HWND hWnd, int type, double dx, double dy,
	double cx, double cy, double kx, double ky, double angle);

void changePages(HWND hWnd, int open, int status);

Row* free_memory(Row *start);

void cleanPicture(HWND hWnd);

void addListViewItem(HWND ListView, int row, double x, double y, int t);

bool isStringNumber(string s);

void setCharText(HWND hwnd, char* text);

void DrawLine(HWND hWnd, int X1, int Y1, int X2, int Y2, int type);

void scaleXY(double x, double y, double xc, double yc, double *x_new, double *y_new, double kx, double ky);

void doubleToChar(const char* text, double coord, char* result);

void greatRewrite(HWND hWnd);

void createModul(HWND hWnd, BOOL NEW, BOOL MENU);

void unitChar(const char* text, const char* str, char* result);

void printOutside(int a, int b, char* message);

double invert_y(HWND hWnd, double y);

int is_small(string s);

void cleanRect(HWND hWnd, LONG left, LONG top, LONG right, LONG bottom);

void globalControl(HWND hWnd);

void setWindowDoubleText(HWND hWnd, int element, int x, int y, const char* text, double coord);

Row* deleteLastFromList(Row *list);

void printDouble(HWND hWnd, int x, int y, int weight, int height, double coord);

void textOutControl(HWND hWnd);

int getTableSize(Row *list);

void printError(HWND hWnd, int number);

void launchXY(Row* list, double *change_x, double *change_y);

void drawPicture(HWND hWnd);

Row* saveInfo(HWND hWnd, Row* row, int type);

void globalLook(HWND hWnd, Row* list);

void drawRect(HWND hWnd, double xc, double yc, double width, double height, BOOL direct);

void drawEllipse(HWND hWnd, double xc, double yc, double width, double height, BOOL direct, BOOL diagonal);

double global_rotate = 0;
double global_dx = 0;
double global_dy = 0;
double global_kx = 1;
double global_ky = 1;
double global_cx = 350;
double global_cy = 280;

CONST double PI = 3.1415926535;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_KGLAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KGLAB2));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_KGLAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KGLAB2);
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
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
	{
		createModul(hWnd, TRUE, FALSE);

		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		cleanRect(hWnd, 0, 0, 0, 0);
		drawPicture(hWnd);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int mouse_click_x = LOWORD(lParam);
		int mouse_click_y = HIWORD(lParam);

		if (mouse_click_x < WidthGrid + XStartPos)
		{
			cleanRect(hWnd, 0, 0, WidthGrid + XStartPos + 150, 0);

			setWindowDoubleText(hWnd, 0, mouse_click_x + 10, mouse_click_y, "X: ", mouse_click_x);
			setWindowDoubleText(hWnd, 0, mouse_click_x + 10, mouse_click_y + 20, "Y: ", invert_y(hWnd, mouse_click_y));
		}
		drawPicture(hWnd);
		break;
	}
	// перемещение окна
	case WM_SIZE:
	{
		greatRewrite(hWnd);

		break;
	}
	case WM_COMMAND:
	{
		HWND button_scale = GetDlgItem(hWnd, ID_BUTTON_SCALE);
		HWND button_rotate = GetDlgItem(hWnd, ID_BUTTON_ROTATE);
		HWND button_move = GetDlgItem(hWnd, ID_BUTTON_MOVE);
		HWND button_cancel = GetDlgItem(hWnd, ID_BUTTON_CANCEL);
		HWND button_save = GetDlgItem(hWnd, ID_BUTTON_SAVE);
		HWND button_center = GetDlgItem(hWnd, ID_BUTTON_CENTER);
		HWND button_back = GetDlgItem(hWnd, ID_BUTTON_BACK);
		HWND button_original = GetDlgItem(hWnd, ID_BUTTON_ORIGINAL);

		int wmId = LOWORD(wParam);
		
		switch (wmId)
		{
		case ID_BUTTON_CENTER:
		{
			if (IsWindowVisible(button_center))
			{
				globalLook(hWnd, listRow);
				setWindowDoubleText(hWnd, ID_EDIT_CDX, 0, 0, "", global_cx);
				setWindowDoubleText(hWnd, ID_EDIT_CDY, 0, 0, "", invert_y(hWnd, global_cy));
			}
			break;
		}
		case ID_BUTTON_BACK:
		{
			if (IsWindowVisible(button_back))
			{
				listRow = deleteLastFromList(listRow);
				cleanRect(hWnd, 0, 0, 0, 0);
				createModul(hWnd, FALSE, TRUE);
				page = ID_MODE_MENU;
				drawPicture(hWnd);
			}
			break;
		}
		case ID_BUTTON_ORIGINAL:
		{
			if (IsWindowVisible(button_original))
			{
				while (listRow->type != 0)
				{
					listRow = deleteLastFromList(listRow);
				}
				cleanRect(hWnd, 0, 0, 0, 0);
				createModul(hWnd, FALSE, TRUE);
				page = ID_MODE_MENU;
				drawPicture(hWnd);
			}
			break;
		}
		case ID_BUTTON_SCALE:
		{
			if (IsWindowVisible(button_scale))
			{
				changePages(hWnd, 1, ID_MODE_SCALING);
			}
			break;
		}
		case ID_BUTTON_ROTATE:
		{
			if (IsWindowVisible(button_rotate))
			{
				changePages(hWnd, 1, ID_MODE_ROTATING);
			}
			break;
		}
		case ID_BUTTON_MOVE:
		{
			if (IsWindowVisible(button_move))
			{
				changePages(hWnd, 1, ID_MODE_MOVING);
			}
			break;
		}
		case ID_BUTTON_CANCEL:
		{
			if (IsWindowVisible(button_cancel))
			{
				changePages(hWnd, 0, ID_MODE_MENU);
			}
			break;
		}
		case ID_BUTTON_SAVE:
		{
			TCHAR save_name[BUFFER_MAX_SIZE];
			GetWindowTextA(button_save, (LPSTR)save_name, sizeof(save_name));
			int type = 0;
			Row* saveInfo(HWND hWnd, Row* row, int type);
			if (strcmp("Подвинуть", save_name) == 0)
			{
				type = ID_MODE_MOVING;
			}
			else if (strcmp("Отмасштабировать", save_name) == 0)
			{
				type = ID_MODE_SCALING;
			}
			else if (strcmp("Повернуть", save_name) == 0)
			{
				type = ID_MODE_ROTATING;
			}
			listRow = saveInfo(hWnd, listRow, type);
			changePages(hWnd, 0, ID_MODE_MENU);
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
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Объединить два текста, ничего не повредив
void unitChar(const char* text, const char* str, char* result)
{
	TCHAR textt[BUFFER_MAX_SIZE];
	strcpy_s(textt, text);
	strcat_s(textt, str);

	for (int i = 0; i < strlen(textt) + 1; i++)
	{
		result[i] = textt[i];
	}
}

// Перевод double в char. Наконецто!
void doubleToChar(const char* text, double coord, char* result)
{
	TCHAR str[BUFFER_MAX_SIZE];
	TCHAR textt[BUFFER_MAX_SIZE];
	sprintf_s(str, BUFFER_MAX_SIZE - 1, "%f", coord);
	strcpy_s(textt, text);
	strcat_s(textt, str);

	for (int i = 0; i < strlen(textt) + 1; i++)
	{
		result[i] = textt[i];
	}
}

// Перерисовка экрана
void greatRewrite(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	MoveWindow(GetDlgItem(hWnd, ID_TEXT_LABEL), rc.right - 470, 2, 300, 20, 1);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_BACK), rc.right - 440, rc.bottom - 170, 80, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ORIGINAL), rc.right - 360, rc.bottom - 170, 190, 20, 1);
	
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_SCALE), rc.right - 300, 2, 150, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ROTATE), rc.right - 150, 2, 70, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_MOVE), rc.right - 80, 2, 78, 20, 1);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_SAVE), rc.right - 250, 2, 150, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CANCEL), rc.right - 100, 2, 100, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CENTER), rc.right - 380, 100, 150, 20, 1);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_CDX), rc.right - 380, 50, 150, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_CDY), rc.right - 380, 75, 150, 20, 1);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_KX), rc.right - 180, 50, 150, 20, 1);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_KAY), rc.right - 180, 75, 150, 20, 1);
}

// Поместить double в текстовые поля
void setWindowDoubleText(HWND hWnd, int element, int x, int y, const char* text, double coord)
{
	TCHAR textt[BUFFER_MAX_SIZE];
	doubleToChar(text, coord, textt);

	if (element > 0)
	{
		SetWindowText(GetDlgItem(hWnd, element), textt);
	}
	else if (element == 0)
	{
		HDC hdc = GetDC(hWnd);
		TextOut(hdc, x, y, textt, strlen(textt));
	}
}

// Печать сообщения об ошибке
void printError(HWND hWnd, int number)
{
	switch (number)
	{
	case ERROR_NOT_ENOUGH_POINTS_FIRST:
	{
		MessageBox(hWnd, "Not enough points of first type", "UserError", MB_OK);
		break;
	}
	case ERROR_NOT_ENOUGH_POINTS_SECOND:
	{
		MessageBox(hWnd, "Not enough points of second type", "UserError", MB_OK);
		break;
	}
	case ERROR_NO_FIRST_TOP:
	{
		MessageBox(hWnd, "No top of first type", "UserError", MB_OK);
		break;
	}
	case ERROR_NO_SECOND_TOP:
	{
		MessageBox(hWnd, "No top of second type", "UserError", MB_OK);
		break;
	}
	}
}

// Инициализация структуры
Row* init_row()
{
	Row *row = (Row*)(calloc(1, sizeof(Row)));
	row->type = 0;
	row->dx = 0;
	row->dy = 0;
	row->cx = 0;
	row->cy = 0;
	row->kx = 0;
	row->ky = 0;
	row->angle = 0;
	row->next = NULL;

	return row;
}

// Добавить в список
Row* add_to_list(Row *list, HWND hWnd,  int type, double dx, double dy,
	double cx, double cy, double kx, double ky, double angle)
{
	Row *row = NULL, *row_prev = NULL;
	Row* to_add;
	if (!list)
	{
		list = init_row();
	}
	if (list->type == 0)
	{
		to_add = list;
	}
	else
	{
		to_add = init_row();
	}
	to_add->type = type;
	switch (type)
	{
		case ID_MODE_MOVING:
		{
			to_add->dx = dx;
			to_add->dy = dy;
			break;
		}
		case ID_MODE_ROTATING:
		{
			to_add->cx = cx;
			to_add->cy = invert_y(hWnd, cy);
			to_add->angle = angle;
			break;
		}
		case ID_MODE_SCALING:
		{
			to_add->cx = cx;
			to_add->cy = invert_y(hWnd, cy);
			to_add->kx = kx;
			to_add->ky = ky;
			break;
		}
		default:
		{
			MessageBox(NULL, "Found invalid type in add_to_list", "NeverError", MB_OK);
			return NULL;
		}
	}

	if (to_add == list)
	{
		return to_add;
	}

	row = list;
	while (row != NULL)
	{
		row_prev = row;
		row = row->next;
	}

	if (row_prev)
	{
		row_prev->next = to_add;
	}
	else
	{
		MessageBox(NULL, "Bad situation in add_to_list", "NeverError", MB_OK);
		return NULL;
	}
	return list;
}

// Удалить последний элемент из списка
Row* deleteLastFromList(Row *list)
{
	Row *row = list;

	if (!list)
	{
		MessageBox(NULL, "List can not be NULL", "NeverError", MB_OK);
		return list;
	}
	if (!list->next)
	{
		list->type = 0;
		//MessageBox(NULL, "Almost clean", "Message", MB_OK);
		return list;
	}

	Row *prev = NULL;
	while (row->next != NULL)
	{
		prev = row;
		row = row->next;
	}
	free(row);

	prev->next = NULL;

	return list;
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

void textOutControl(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	TCHAR text_move[BUFFER_MAX_SIZE] = "Cмещение по осям X и Y";
	TCHAR text_rotate[BUFFER_MAX_SIZE] = "Центр поворота и угол поворота";
	TCHAR text_scale1[BUFFER_MAX_SIZE] = "Центр масштабирования";
	TCHAR text_scale2[BUFFER_MAX_SIZE] = "Коэффициенты масштабирования";
	TCHAR text_dx[BUFFER_MAX_SIZE] = "dX";
	TCHAR text_dy[BUFFER_MAX_SIZE] = "dY";
	TCHAR text_kx[BUFFER_MAX_SIZE] = "kX";
	TCHAR text_ky[BUFFER_MAX_SIZE] = "kY";
	TCHAR text_x[BUFFER_MAX_SIZE] = "X";
	TCHAR text_y[BUFFER_MAX_SIZE] = "Y";
	TCHAR text_angle[BUFFER_MAX_SIZE] = "угол";
	TCHAR text_grad[BUFFER_MAX_SIZE] = "(в градусах)";

	switch (page)
	{
		case ID_MODE_MOVING:
		{
			TextOut(GetDC(hWnd), rc.right - 380, 30, text_move, strlen(text_move));
			TextOut(GetDC(hWnd), rc.right - 400, 53, text_dx, strlen(text_dx));
			TextOut(GetDC(hWnd), rc.right - 400, 78, text_dy, strlen(text_dy));
			break;
		}
		case ID_MODE_ROTATING:
		{
			TextOut(GetDC(hWnd), rc.right - 380, 30, text_rotate, strlen(text_rotate));
			TextOut(GetDC(hWnd), rc.right - 400, 53, text_x, strlen(text_x));
			TextOut(GetDC(hWnd), rc.right - 400, 78, text_y, strlen(text_y));
			TextOut(GetDC(hWnd), rc.right - 155, 53, text_grad, strlen(text_grad));
			TextOut(GetDC(hWnd), rc.right - 225, 78, text_angle, strlen(text_angle));
			break;
		}
		case ID_MODE_SCALING:
		{
			TextOut(GetDC(hWnd), rc.right - 440, 30, text_scale1, strlen(text_scale1));
			TextOut(GetDC(hWnd), rc.right - 240, 30, text_scale2, strlen(text_scale2));
			TextOut(GetDC(hWnd), rc.right - 400, 53, text_x, strlen(text_x));
			TextOut(GetDC(hWnd), rc.right - 400, 78, text_y, strlen(text_y));
			TextOut(GetDC(hWnd), rc.right - 200, 53, text_kx, strlen(text_kx));
			TextOut(GetDC(hWnd), rc.right - 200, 78, text_ky, strlen(text_ky));
			break;
		}
	}
}

// Получить граничные координаты рисунка
void getBorders(HWND hWnd, LONG *left, LONG *top, LONG *right, LONG *bottom)
{
	globalControl(hWnd);
	double width = 300 * global_kx;
	double height = invert_y(hWnd, 250) * global_ky;
	*left = global_cx - width / 2;
	*right = global_cx + width / 2;
	*top = global_cy - height / 2;
	*bottom = global_cx + height / 2;
}

// Очистить картину
void cleanPicture(HWND hWnd)
{
	LONG left, right, top, bottom;
	getBorders(hWnd, &left, &top, &right, &bottom);
	cleanRect(hWnd, left / 1.5, top / 1.5, right * 1.5, bottom * 1.5);
}

//  Перемещение между страницами
void changePages(HWND hWnd, int open, int status)
{
	HWND button_scale = GetDlgItem(hWnd, ID_BUTTON_SCALE);
	HWND button_rotate = GetDlgItem(hWnd, ID_BUTTON_ROTATE);
	HWND button_move = GetDlgItem(hWnd, ID_BUTTON_MOVE);

	HWND button_original = GetDlgItem(hWnd, ID_BUTTON_ORIGINAL);
	HWND button_back = GetDlgItem(hWnd, ID_BUTTON_BACK);

	HWND button_cancel = GetDlgItem(hWnd, ID_BUTTON_CANCEL);
	HWND button_save = GetDlgItem(hWnd, ID_BUTTON_SAVE);
	HWND edit_text = GetDlgItem(hWnd, ID_TEXT_LABEL);
	HWND button_center = GetDlgItem(hWnd, ID_BUTTON_CENTER);

	HWND edit_cdx = GetDlgItem(hWnd, ID_EDIT_CDX);
	HWND edit_cdy = GetDlgItem(hWnd, ID_EDIT_CDY);
	HWND edit_kx = GetDlgItem(hWnd, ID_EDIT_KX);
	HWND edit_kay = GetDlgItem(hWnd, ID_EDIT_KAY);

	cleanRect(hWnd, 0, 0, 0, 0);

	RECT rc;
	GetClientRect(hWnd, &rc);

	DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_ORIGINAL));
	DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_BACK));

	CreateWindow("button", "Шаг назад", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		rc.right - 440, rc.bottom - 170, 80, 20, hWnd, (HMENU)ID_BUTTON_BACK, NULL, NULL);
	CreateWindow("button", "К оригиналу", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		rc.right - 360, rc.bottom - 170, 190, 20, hWnd, (HMENU)ID_BUTTON_ORIGINAL, NULL, NULL);

	ShowWindow(button_scale, !open);
	ShowWindow(button_rotate, !open);
	ShowWindow(button_move, !open);
	ShowWindow(button_center, open);

	ShowWindow(button_cancel, open);
	ShowWindow(button_save, open);

	ShowWindow(edit_cdx, open);
	ShowWindow(edit_cdy, open);

	page = status;
	switch (status)
	{
		case ID_MODE_MOVING:
		{
			SetWindowText(button_save, "Подвинуть");
			SetWindowText(edit_text, "Раздел перемещения:");
			ShowWindow(edit_kx, FALSE);
			ShowWindow(edit_kay, FALSE);
			break;
		}
		case ID_MODE_ROTATING:
		{
			SetWindowText(button_save, "Повернуть");
			SetWindowText(edit_text, "Раздел вращения:");
			ShowWindow(edit_kx, FALSE);
			ShowWindow(edit_kay, TRUE);
			break;
		}
		case ID_MODE_SCALING:
		{
			SetWindowText(button_save, "Отмасштабировать");
			SetWindowText(edit_text, "Раздел масштабирования:");
			ShowWindow(edit_kx, TRUE);
			ShowWindow(edit_kay, TRUE);
			break;
		}
		case ID_MODE_MENU:
		{
			SetWindowText(button_save, "Сохранить");
			SetWindowText(edit_text, "Выберите действие:");
			ShowWindow(edit_kx, FALSE);
			ShowWindow(edit_kay, FALSE);
			ShowWindow(edit_cdx, FALSE);
			ShowWindow(edit_cdy, FALSE);
			break;
		}
	}
}

// Создание элементов
void createModul(HWND hWnd, BOOL NEW, BOOL MENU)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	TCHAR text_answer_text[BUFFER_MAX_SIZE] = " ";
	TCHAR text_label[BUFFER_MAX_SIZE] = "Выберите действие:";
	TCHAR text_edit[BUFFER_MAX_SIZE] = "edit";
	TCHAR text_button[BUFFER_MAX_SIZE] = "button";
	TCHAR text_rotate[BUFFER_MAX_SIZE] = "поворот";
	TCHAR text_scale[BUFFER_MAX_SIZE] = "масштабирование";
	TCHAR text_save[BUFFER_MAX_SIZE] = "Сохранить";
	TCHAR text_cancel[BUFFER_MAX_SIZE] = "Отмена";
	TCHAR text_move[BUFFER_MAX_SIZE] = "движение";
	TCHAR text_action[BUFFER_MAX_SIZE] = "действие";
	TCHAR text_center[BUFFER_MAX_SIZE] = "Центр картинки";
	TCHAR text_back[BUFFER_MAX_SIZE] = "Шаг назад";
	TCHAR text_original[BUFFER_MAX_SIZE] = "К оригиналу";

	if (MENU)
	{
		DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_SCALE));
		DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_ROTATE));
		DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_MOVE));

		DestroyWindow(GetDlgItem(hWnd, ID_TEXT_LABEL));
		DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_ORIGINAL));
		DestroyWindow(GetDlgItem(hWnd, ID_BUTTON_BACK));

		CreateWindow(text_edit, text_label, WS_CHILD | WS_VISIBLE,
			rc.right - 400, 2, 100, 20, hWnd, (HMENU)ID_TEXT_LABEL, NULL, NULL);

		EnableWindow(GetDlgItem(hWnd, ID_TEXT_LABEL), FALSE);
		SendDlgItemMessage(hWnd, ID_TEXT_LABEL, EM_SETREADONLY, 1, 0);

		CreateWindow(text_button, text_back, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 440, rc.bottom - 170, 80, 20, hWnd, (HMENU)ID_BUTTON_BACK, NULL, NULL);
		CreateWindow(text_button, text_original, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 360, rc.bottom - 170, 190, 20, hWnd, (HMENU)ID_BUTTON_ORIGINAL, NULL, NULL);

		CreateWindow(text_button, text_scale, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 300, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_SCALE, NULL, NULL);
		CreateWindow(text_button, text_rotate, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_ROTATE, NULL, NULL);
		CreateWindow(text_button, text_move, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 100, 2, 98, 20, hWnd, (HMENU)ID_BUTTON_MOVE, NULL, NULL);
	}
	if (NEW)
	{
		CreateWindow(text_button, text_back, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 440, rc.bottom - 170, 80, 20, hWnd, (HMENU)ID_BUTTON_BACK, NULL, NULL);
		CreateWindow(text_button, text_original, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 360, rc.bottom - 170, 190, 20, hWnd, (HMENU)ID_BUTTON_ORIGINAL, NULL, NULL);

		CreateWindow(text_edit, text_label, WS_CHILD | WS_VISIBLE,
			rc.right - 400, 2, 100, 20, hWnd, (HMENU)ID_TEXT_LABEL, NULL, NULL);

		CreateWindow(text_button, text_scale, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 300, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_SCALE, NULL, NULL);
		CreateWindow(text_button, text_rotate, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_ROTATE, NULL, NULL);
		CreateWindow(text_button, text_move, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 100, 2, 98, 20, hWnd, (HMENU)ID_BUTTON_MOVE, NULL, NULL);

		CreateWindow(text_button, text_save, WS_CHILD | BS_PUSHBUTTON,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
		CreateWindow(text_button, text_cancel, WS_CHILD | BS_PUSHBUTTON,
			rc.right - 100, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_CANCEL, NULL, NULL);
		CreateWindow(text_button, text_center, WS_CHILD | WS_BORDER,
			rc.right - 380, 100, 150, 20, hWnd, (HMENU)ID_BUTTON_CENTER, NULL, NULL);

		CreateWindow(text_edit, "0", WS_CHILD | WS_BORDER,
			rc.right - 380, 50, 150, 20, hWnd, (HMENU)ID_EDIT_CDX, NULL, NULL);
		CreateWindow(text_edit, "0", WS_CHILD | WS_BORDER,
			rc.right - 380, 75, 150, 20, hWnd, (HMENU)ID_EDIT_CDY, NULL, NULL);

		CreateWindow(text_edit, "0", WS_CHILD | WS_BORDER,
			rc.right - 180, 50, 150, 20, hWnd, (HMENU)ID_EDIT_KX, NULL, NULL);
		CreateWindow(text_edit, "0", WS_CHILD | WS_BORDER,
			rc.right - 180, 75, 150, 20, hWnd, (HMENU)ID_EDIT_KAY, NULL, NULL);

		EnableWindow(GetDlgItem(hWnd, ID_TEXT_LABEL), FALSE);

		SendDlgItemMessage(hWnd, ID_TEXT_LABEL, EM_SETREADONLY, 1, 0);
	}
}

// Очистить область
void cleanRect(HWND hWnd, LONG left, LONG top, LONG right, LONG bottom)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	if (left > 0)
		rect.left = left;
	if (right > 0)
		rect.right = right;
	if (top > 0)
		rect.top = top;
	if (bottom > 0)
		rect.bottom = bottom;

	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

void addListViewItem(HWND ListView, int row, double x, double y, int t)
{
	LVITEM lis;
	lis.lParam = 0;

	TCHAR buffer_x[BUFFER_MAX_SIZE];
	TCHAR buffer_y[BUFFER_MAX_SIZE];
	TCHAR buffer_t[BUFFER_MAX_SIZE];

	_snprintf_s(buffer_x, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%f", x);

	_snprintf_s(buffer_y, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%f", y);

	_snprintf_s(buffer_t, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", t);

	memset(&lis, 0, sizeof(LV_ITEM));

	lis.iItem = row;
	lis.iSubItem = 0;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 0, buffer_x);

	lis.iSubItem = 1;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 1, buffer_y);

	lis.iSubItem = 2;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 2, buffer_t);
}

// Проверка на переполнение типа
int is_small(string s)
{
	if (s.length() > 8)
	{
		return 0;
	}
	return 1;
}

// Очистка памяти из под списка
Row* free_memory(Row *start)
{
	Row *row = NULL, *tmp = NULL;

	while (row != NULL)
	{
		tmp = row;
		row = row->next;
		free(tmp);
	}

	return NULL;
}

// Проверка, является ли строка числом http://www.cyberforum.ru/cpp-beginners/thread1858629.html
bool isStringNumber(string s)
{
	if (s.length() == 0)
	{
		return false;
	}
	for (int a = 0; a < s.length(); a++)
	{
		// Если в строке есть не цифровые символы, то это не число
		if (s[a] != '-')
		{ // Проверка на знак минус
			if (s[a] != '.') // Проверка на точку
			{
				if ((s[a] < 48) || (s[a] > 57))
				{
					return false;
				}
			}
		}
	} // Если в строке только цифровые символы значит это число
	return true;
}

// Установка обычного текста
// https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
// https://stackoverflow.com/questions/29847036/convert-char-to-wchar-t-using-mbstowcs-s
void setCharText(HWND hwnd, char* text)
{
	wchar_t wtext[BUFFER_MAX_SIZE];
	size_t outSize;
	size_t size = strlen(text) + 1;
	mbstowcs_s(&outSize, wtext, size, text, size - 1);//Plus null
	LPWSTR ptr = wtext;
	SetWindowText(hwnd, text);
}

// Получить количество элементов в списке
int getTableSize(Row *list)
{
	int size = 0;
	Row *row = list;
	while (row != NULL)
	{
		size++;
		row = row->next;
	}
	
	return size;
}

// Инвертирование y
double invert_y(HWND hWnd, double y)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	return rc.bottom - 20 + y * (-1) + 15;
}

// Вывод вещественного число в окно
void printDouble(HWND hWnd, int x, int y, int weight, int height, double coord)
{
	TCHAR buffer[BUFFER_MAX_SIZE] = "";
	_snprintf_s(buffer, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%f", coord);

	HWND label = CreateWindow("edit", buffer, WS_CHILD | WS_VISIBLE,
		x, y + 30, weight, height, hWnd, (HMENU)ID_BUTTON_MOVE, NULL, NULL);
	EnableWindow(label, FALSE);
}

// Получить double из текстового поля
double getInfo(HWND element)
{
	TCHAR text[BUFFER_MAX_SIZE];
	GetWindowText(element, text, sizeof(text));

	if (!isStringNumber(text))
	{
		MessageBox(NULL, "Found wrong symbols in first text field", "IOError", MB_OK);
		return 0;
	}

	double value = atof(text);

	return value;
}

// Сохранить в список
Row* saveInfo(HWND hWnd, Row* row, int type)
{
	HWND edit_cdx = GetDlgItem(hWnd, ID_EDIT_CDX);
	HWND edit_cdy = GetDlgItem(hWnd, ID_EDIT_CDY);
	HWND edit_kx = GetDlgItem(hWnd, ID_EDIT_KX);
	HWND edit_kay = GetDlgItem(hWnd, ID_EDIT_KAY);

	double cdx = getInfo(edit_cdx);
	double cdy = getInfo(edit_cdy);
	double kx = getInfo(edit_kx);
	double kay = getInfo(edit_kay);

	switch (type)
	{
		case ID_MODE_ROTATING:
		{
			row = add_to_list(row, hWnd, type, 0, 0, cdx, cdy, 0, 0, kay / 180 * PI);
			break;
		}
		case ID_MODE_SCALING:
		{
			row = add_to_list(row, hWnd, type, 0, 0, cdx, cdy, kx, kay, 0);
			break;
		}
		case ID_MODE_MOVING:
		{
			row = add_to_list(row, hWnd, type, cdx, cdy, 0, 0, 0, 0, 0);
			break;
		}
	}
	return row;
}

// Рисование линии для сетки
void DrawLine(HWND hWnd, int X1, int Y1, int X2, int Y2, int type)
{
	HDC hdc = GetDC(hWnd);
	HPEN p1, p2;
	if (type == 0)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	}
	if (type == 1)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	}
	else if (type == 2)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	}
	else if (type == 3)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
	}
	else
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	}
	p1 = (HPEN)SelectObject(hdc, p2);  // Заносим красное перо в контекст рисования
	MoveToEx(hdc, X1, Y1, NULL);
	LineTo(hdc, X2, Y2);    // Проводим линию
	SelectObject(hdc, p1);   // Возвращаем старое перо
	DeleteObject(p2);       // Удаляем созданное перо
}

// Печать чисел как текста на экране
void printOutside(int a, int b, char* message)
{
	TCHAR buffer[BUFFER_MAX_SIZE];
	wsprintf(buffer, "%d %d", a, b);
	MessageBox(NULL, buffer, message, MB_OK);
}

// Масштабирование
void scaleXY(double x, double y, double xc, double yc, double *x_new, double *y_new, double kx, double ky)
{
	*x_new = x * kx + (1 - kx) * xc;
	*y_new = y * ky + (1 - ky) * yc;
}

// Управление глобальными переменными
void globalControl(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	globalLook(hWnd, listRow);
	HDC hdc = GetDC(hWnd);
	TextOut(hdc, rc.right - 440, rc.bottom - 200, "Центр картины", strlen("Центр картины"));
	setWindowDoubleText(hWnd, 0, rc.right - 340, rc.bottom - 200, "[", global_cx);
	setWindowDoubleText(hWnd, 0, rc.right - 260, rc.bottom - 200, ", ", invert_y(hWnd, global_cy));
	TextOut(hdc, rc.right - 175, rc.bottom - 200, "]", strlen("]"));

	TextOut(hdc, rc.right - 440, rc.bottom - 150, "Изменения по сравнению с оригиналом", strlen("Изменения по сравнению с оригиналом"));
	setWindowDoubleText(hWnd, 0, rc.right - 440, rc.bottom - 120, "Угол поворота                    : ", global_rotate / PI * 180);
	setWindowDoubleText(hWnd, 0, rc.right - 440, rc.bottom - 100, "Смещение по X                   : ", 300 - global_cx);
	setWindowDoubleText(hWnd, 0, rc.right - 440, rc.bottom - 80,  "Смещение по Y                   : ", 250 - global_cy);
	setWindowDoubleText(hWnd, 0, rc.right - 440, rc.bottom - 60,  "Коэффициент масштабирования по X: ", global_kx);
	setWindowDoubleText(hWnd, 0, rc.right - 440, rc.bottom - 40,  "Коэффициент масштабирования по Y: ", global_ky);
}

void anime(HWND hWnd, Row* list)
{
	double copy_dx, copy_dy, copy_kx, copy_kay;
	Row* row = list;
	while (row != NULL)
	{
		copy_dx = 0;
		copy_dy = 0;
		copy_kx = 1;
		copy_kay = 1;
		switch (row->type)
		{
			case ID_MODE_MOVING:
			{
				copy_dx = row->dx;
				copy_dy = row->dy;
				break;
			}
			case ID_MODE_ROTATING:
			{
				copy_kay = row->angle;
				break;
			}
			case ID_MODE_SCALING:
			{
				copy_kx = row->kx;
				copy_kay = row->ky;
				break;
			}
			default:
				return;
		}
		int i = 0;
		while (i < 11)
		{
			row->dx = copy_dx * i / 10;
			row->dy = copy_dy * i / 10;
			row->kx = copy_kx * i / 10;
			row->ky = copy_kay * i / 10;
			row->angle = copy_kay * i / 10;
			cleanPicture(hWnd);

			drawRect(hWnd, 300, 300, 300, 200, FALSE);
			drawRect(hWnd, 300, 160, 60, 40, TRUE);

			drawEllipse(hWnd, 350, 300, 80, 150, TRUE, TRUE);

			drawEllipse(hWnd, 200, 250, 60, 60, TRUE, FALSE);
			i++;
		}
		row = row->next;
	}
}

// Рисование точки
void drawPoint(HWND hWnd, double XBuf, double YBuf, int radius, int type)
{
	XBuf = fabs(XBuf);
	YBuf = fabs(YBuf);
	bool flag = true;
	for (register int Y = 0, count_y = YStartPos + HeightGrid; Y <= count_y && flag; Y += StepGrid)
	{
		for (register int X = XStartPos, count_x = XStartPos + WidthGrid; X <= count_x; X += StepGrid)
		{
			if ((XBuf >= X && XBuf <= X + StepGrid) && (YBuf >= Y && YBuf <= Y + StepGrid))
			{
				HDC hdc = GetDC(hWnd);
				HBRUSH hBrush;
				if (type == 1)
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
				}
				else if (type == 2)
				{
					hBrush = CreateSolidBrush(RGB(0, 255, 0));
				}
				else if (type == 3)
				{
					hBrush = CreateSolidBrush(RGB(0, 0, 255));
				}
				else if (type == 4)
				{
					hBrush = CreateSolidBrush(RGB(0, 255, 255));
				}
				else if (type == 5)
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 255));
				}
				else
				{
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					MessageBox(hWnd, "Wrong type in point", "Warning", MB_OK);
				}
				SelectObject(hdc, hBrush);
				Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);
				DeleteObject(hBrush);
				flag = false;
				break;
			}
		}
	}
}

// Рисование рисунка
void drawPicture(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	
	greatRewrite(hWnd);

	if (listRow == NULL)
	{
		listRow = init_row();
	}

	//anime(hWnd, listRow);
	
	drawRect(hWnd, 300, 300, 300, 200, FALSE);
	drawRect(hWnd, 300, 160, 60, 40, TRUE);

	drawEllipse(hWnd, 350, 300, 80, 150, TRUE, TRUE);

	drawEllipse(hWnd, 200, 250, 60, 60, TRUE, FALSE);
	
	double x1 = 300, y1 = 100, x2 = 450, y2 = 200, x3 = 150, y3 = 200;
	launchXY(listRow, &x1, &y1);
	launchXY(listRow, &x2, &y2);
	launchXY(listRow, &x3, &y3);

	DrawLine(hWnd, x1, y1, x2, y2, 2);
	DrawLine(hWnd, x3, y3, x2, y2, 2);
	DrawLine(hWnd, x1, y1, x3, y3, 2);
	
	if (listRow->type == 0)
	{
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_BACK), FALSE);
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_ORIGINAL), FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_BACK), TRUE);
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_ORIGINAL), TRUE);
	}

	double xp = 300;
	double yp = 250;

	textOutControl(hWnd);
	globalControl(hWnd);
	
	launchXY(listRow, &xp, &yp);
	drawPoint(hWnd, xp, yp, 5, 1);
}

// Поворот координат x,y вокруг xc, yc на угол angle
void rotateXY(double x, double y, double xc, double yc, double *x_new, double *y_new, double angle)
{
	*x_new = xc + (x - xc) * cos(angle) + (y - yc) * sin(angle);
	*y_new = yc - (x - xc) * sin(angle) + (y - yc) * cos(angle);
}

// Рисование эллипса http://www.cyberforum.ru/cpp-builder/thread1761351.html
void drawEllipse(HWND hWnd, double xc, double yc, double width, double height, BOOL direct, BOOL diagonal)
{
	HDC hdc = GetDC(hWnd);

	double x1 = xc - width / 2;
	double y1 = yc;
	double x2 = xc + width / 2;
	double y2 = yc;
	double x3 = xc;
	double y3 = yc - height / 2;
	double x4 = xc;
	double y4 = yc + height / 2;

	launchXY(listRow, &x1, &y1);
	launchXY(listRow, &x2, &y2);
	launchXY(listRow, &x3, &y3);
	launchXY(listRow, &x4, &y4);

	if (direct)
	{
		DrawLine(hWnd, x1, y1, x2, y2, 3);
		DrawLine(hWnd, x3, y3, x4, y4, 3);
	}
	if (diagonal)
	{
		DrawLine(hWnd, x1, y1, x3, y3, 3);
		DrawLine(hWnd, x2, y2, x3, y3, 3);
		DrawLine(hWnd, x1, y1, x4, y4, 3);
		DrawLine(hWnd, x2, y2, x4, y4, 3);
	}
	double x, y;
	for (double t = 0; t <= 2 * PI; t += 0.001)
	{
		x = xc + width / 2 * cos(t);
		y = yc + height / 2 * sin(t);
		launchXY(listRow, &x, &y);
		SetPixel(hdc, x, y, RGB(255, 0, 205));
	}
}

// Для обновления глобальных данных
void globalLook(HWND hWnd, Row* list)
{
	global_dx = 0;
	global_dy = 0;
	global_kx = 1;
	global_ky = 1;
	global_cx = 300;
	global_cy = 250;

	launchXY(listRow, &global_dx, &global_dy);
	launchXY(listRow, &global_cx, &global_cy);

	global_rotate = 0;

	Row* row = list;

	while (row != NULL)
	{
		switch (row->type)
		{
			case ID_MODE_SCALING:
			{
				global_kx *= row->kx;
				global_ky *= row->ky;
				break;
			}
			case ID_MODE_ROTATING:
			{
				global_rotate += row->angle;
				global_kx *= row->kx;
				global_ky *= row->ky;
				break;
			}
		}
		row = row->next;
	}
}

// Посылать сюда нужно копии, чтобы всегда надержать наготове несломанный оригинал
void launchXY(Row* list, double *change_x, double *change_y)
{
	Row* row = list;
	double x = *change_x, y = *change_y;

	global_rotate = 0;
	global_kx = 1;
	global_dx = 0;
	global_dy = 0;
	global_kx = 1;
	global_ky = 1;
	global_cx = 300;
	global_cy = 250;

	while (row != NULL)
	{
		switch (row->type)
		{
			case ID_MODE_MOVING:
			{
				x += row->dx;
				y -= row->dy;
				break;
			}
			case ID_MODE_ROTATING:
			{
				global_rotate += row->angle;
				rotateXY(x, y, row->cx, row->cy, &x, &y, row->angle);
				break;
			}
			case ID_MODE_SCALING:
			{
				scaleXY(x, y, row->cx, row->cy, &x, &y, row->kx, row->ky);
				break;
			}
			default:
				return;
		}
		row = row->next;
	}

	*change_x = x;
	*change_y = y;
}

// Рисование прямоугольника
void drawRect(HWND hWnd, double xc, double yc, double width, double height, BOOL direct)
{
	HDC hdc = GetDC(hWnd);

	double x1 = xc - width / 2;
	double y1 = yc - height / 2;
	double x2 = x1;
	double y2 = y1 + height;
	double x3 = x2 + width;
	double y3 = y2;
	double x4 = x3;
	double y4 = y1;

	launchXY(listRow, &x1, &y1);
	launchXY(listRow, &x2, &y2);
	launchXY(listRow, &x3, &y3);
	launchXY(listRow, &x4, &y4);

	if (direct)
	{
		double x_up = xc;
		double y_up = yc - height / 2;
		double x_down = xc;
		double y_down = yc + height / 2;
		double x_right = xc + width / 2;
		double y_right = yc;
		double x_left = xc - width / 2;
		double y_left = yc;

		launchXY(listRow, &x_up, &y_up);
		launchXY(listRow, &x_down, &y_down);
		launchXY(listRow, &x_right, &y_right);
		launchXY(listRow, &x_left, &y_left);

		DrawLine(hWnd, x_up, y_up, x_down, y_down, 1);
		DrawLine(hWnd, x_right, y_right, x_left, y_left, 1);
	}
	DrawLine(hWnd, x1, y1, x2, y2, 1);
	DrawLine(hWnd, x1, y1, x4, y4, 1);
	DrawLine(hWnd, x2, y2, x3, y3, 1);
	DrawLine(hWnd, x4, y4, x3, y3, 1);
}

// 0.1 - 17.02.18 - 1336
// 0.2 - 20.02.18 - 1376