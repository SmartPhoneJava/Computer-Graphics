#include "stdafx.h"
#include "Interface.h"

// Установка обычного текста
// https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
// https://stackoverflow.com/questions/29847036/convert-char-to-wchar-t-using-mbstowcs-s
void setCharText(HWND hwnd, const char* text)
{
	wchar_t wtext[BUFFER_MAX_SIZE];
	size_t outSize;
	size_t size = strlen(text) + 1;
	mbstowcs_s(&outSize, wtext, size, text, size - 1);//Plus null
	LPWSTR ptr = wtext;
	SetWindowText(hwnd, text);
}

// Установка текстом целочисленных значений
void setIntText(HWND hWnd, int ID, int d)
{
	TCHAR buffer[BUFFER_MAX_SIZE];
	_snprintf_s(buffer, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", d);
	setCharText(GetDlgItem(hWnd, ID), buffer);
}

/*

borderX - максмальное значения X
x, y - значения параметров которые обновятся
count - счётчик кликов
*/
void mouseClicked(LPARAM lParam, int borderX,
	int &x, int &y, HWND hWnd, int ID1, int ID2)
{
	if (LOWORD(lParam) < borderX)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		setIntText(hWnd, ID1, x);
		setIntText(hWnd, ID2, y);
	}
}

void addListViewItem(HWND ListView, int x1, int y1, int x2, int y2)
{
	LVITEM lis;
	lis.lParam = 0;

	TCHAR buffer_x1[BUFFER_MAX_SIZE];
	TCHAR buffer_y1[BUFFER_MAX_SIZE];
	TCHAR buffer_x2[BUFFER_MAX_SIZE];
	TCHAR buffer_y2[BUFFER_MAX_SIZE];

	_snprintf_s(buffer_x1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", x1);

	_snprintf_s(buffer_y1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", y1);

	_snprintf_s(buffer_x2, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", x2);

	_snprintf_s(buffer_y2, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", y2);

	memset(&lis, 0, sizeof(LV_ITEM));

	int row = 0;
	lis.iItem = row;
	lis.iSubItem = 0;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 0, buffer_x1);

	lis.iSubItem = 1;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 1, buffer_y1);

	lis.iSubItem = 2;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 2, buffer_x2);

	lis.iSubItem = 3;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 3, buffer_y2);
}

void deleteListViewItem(HWND listview, int i)
{
	ListView_DeleteItem(listview, i);
}

// Получение текста как чисел
int getIntText(HWND hWnd, int ID)
{
	TCHAR buffer[BUFFER_MAX_SIZE];
	GetWindowText(GetDlgItem(hWnd, ID), buffer, sizeof(buffer));
	return atoi(buffer);
}

// Получение координат
void getCoordinates(HWND hWnd, int &x1, int &y1, int &x2, int &y2)
{
	x1 = getIntText(hWnd, ID_EDIT_X1);
	y1 = getIntText(hWnd, ID_EDIT_Y1);
	x2 = getIntText(hWnd, ID_EDIT_X2);
	y2 = getIntText(hWnd, ID_EDIT_Y2);
}

Table* adding(HWND hWnd, Table *table)
{
	int x1 = NO_POINT, x2 = NO_POINT;
	int y1 = NO_POINT, y2 = NO_POINT;

	getCoordinates(hWnd, x1, y1, x2, y2);

	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	addListViewItem(listview, x1, y1, x2, y2);

	Cut *cut = newCutInt(x1, y1, x2, y2);
	return addToTable(table, cut);
}

void updateColumn(int col, LV_ITEM lvi, 
	HWND listview, HWND edit, int &save)
{
	TCHAR buf[BUFFER_MAX_SIZE];
	lvi.iSubItem = col;
	lvi.pszText = buf;
	lvi.cchTextMax = BUFFER_MAX_SIZE - 1;
	ListView_GetItem(listview, &lvi);
	save = atoi(lvi.pszText); //listview_x
	setCharText(edit, lvi.pszText); //edit_x
}

void updateRow(int index, HWND hWnd,
	int &x1, int &y1,
	int &x2, int &y2)
{
	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_IMAGE | LVIF_INDENT |
		LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
	lvi.iItem = index;

	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	updateColumn(0, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_X1), x1);

	updateColumn(1, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_Y1), y1);

	updateColumn(2, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_X2), x2);

	updateColumn(3, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_Y2), y2);
}

void updatingListView(HWND hWnd, LPARAM lParam, 
	int &listview_choosen, int &x1, int &y1,
	int &x2, int &y2, Table* table, Secatel *sec)
{
	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), TRUE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), TRUE);

	NMLISTVIEW * plv;
	plv = (NMLISTVIEW*)lParam;
	plv->iItem;

	int index = ListView_GetNextItem(listview,
		-1, LVNI_ALL | LVNI_SELECTED);

	if (index == -1)
		return;
	listview_choosen = index;

	updateRow(index, hWnd, x1, y1, x2, y2);

	cleanRectOld(hWnd, 0, 0, 300, 0);

	HDC hdc = GetDC(hWnd);

	drawPicture(hWnd, table, sec, RGB(0, 0, 50));

	drawEllipse(hdc, x1, y1, 4, RGB(0, 0, 0));
	drawEllipse(hdc, x1, y1, 3, RGB(200, 200, 200));

	drawEllipse(hdc, x2, y2, 4, RGB(0, 0, 0));
	drawEllipse(hdc, x2, y2, 3, RGB(200, 200, 200));

	ReleaseDC(hWnd, hdc);
}

void deleting(HWND hWnd, int listview_choosen,
	int x1, int y1, int x2, int y2, Table* table)
{
	deleteListViewItem(GetDlgItem(hWnd, ID_LISTVIEW),
		listview_choosen);

	Cut *cut = newCutInt(x1, y1, x2, y2);
	table = deleteOfTable(table, cut);
	deleteCut(&cut);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
}

void changing(HWND hWnd, int listview_choosen,
	Table* table, int old_x1, int old_y1,
	int old_x2, int old_y2)
{
	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);
	deleteListViewItem(listview, listview_choosen);
	
	int x1 = getIntText(hWnd, ID_EDIT_X1);
	int y1 = getIntText(hWnd, ID_EDIT_Y1);
	int x2 = getIntText(hWnd, ID_EDIT_X2);
	int y2 = getIntText(hWnd, ID_EDIT_Y2);

	addListViewItem(listview, x1, y1, x2, y2);

	Cut *oldCut = newCutInt(old_x1, old_y1, old_x2, old_y2);
	Cut *newCut = newCutInt(x1, y1, x2, y2);

	changeTable(table, oldCut, newCut);

	deleteCut(&oldCut);
	deleteCut(&newCut);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
}

void creating(HWND hWnd)
{
	TCHAR text_edit[BUFFER_MAX_SIZE] = "edit";
	TCHAR text_button[BUFFER_MAX_SIZE] = "button";
	TCHAR text_add[BUFFER_MAX_SIZE] = "Добавить";
	TCHAR text_delete[BUFFER_MAX_SIZE] = "Удалить";
	TCHAR text_clear[BUFFER_MAX_SIZE] = "Очистить";
	TCHAR text_build[BUFFER_MAX_SIZE] = "Построить";
	TCHAR text_change[BUFFER_MAX_SIZE] = "Изменить";
	TCHAR text_0[BUFFER_MAX_SIZE] = "0";
	TCHAR text_x1[BUFFER_MAX_SIZE] = "X1";
	TCHAR text_y1[BUFFER_MAX_SIZE] = "Y1";
	TCHAR text_x2[BUFFER_MAX_SIZE] = "X2";
	TCHAR text_y2[BUFFER_MAX_SIZE] = "Y2";
	TCHAR text_x[BUFFER_MAX_SIZE] = "X";
	TCHAR text_y[BUFFER_MAX_SIZE] = "Y";
	TCHAR text_sec [BUFFER_MAX_SIZE] = "Координаты 1-ой вершины отсекателя";
	TCHAR text_status[BUFFER_MAX_SIZE] = "Точки отрезков";

	RECT rc;
	GetClientRect(hWnd, &rc);

	DWORD usual = WS_CHILD | WS_BORDER | WS_VISIBLE | DT_CENTER;

	CreateWindow(text_button, text_add, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
		rc.right - 300, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);
	CreateWindow(text_button, text_delete, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
		rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
	CreateWindow(text_button, text_build, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
		rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_BUILD, NULL, NULL);
	CreateWindow(text_button, text_clear, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
		rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
	CreateWindow(text_button, text_clear, WS_CHILD | BS_PUSHBUTTON,
		rc.right - 200, 22, 100, 20, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
	CreateWindow(text_button, text_change, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		rc.right - 100, 2, 98, 20, hWnd, (HMENU)ID_BUTTON_CHANGE, NULL, NULL);

	CreateWindow(text_edit, text_0, usual, rc.right - 265,
		50, 100, 20, hWnd, (HMENU)ID_EDIT_X1, NULL, NULL);
	CreateWindow(text_edit, text_x1, usual, rc.right - 300,
		50, 30, 20, hWnd, (HMENU)ID_NEDIT_X1, NULL, NULL);
	CreateWindow(text_edit, text_0, usual, rc.right - 265,
		75, 100, 20, hWnd, (HMENU)ID_EDIT_Y1, NULL, NULL);
	CreateWindow(text_edit, text_y1, usual, rc.right - 300,
		75, 30, 20, hWnd, (HMENU)ID_NEDIT_Y1, NULL, NULL);

	CreateWindow(text_edit, text_0, usual,
		rc.right - 115, 50, 100, 20, hWnd, (HMENU)ID_EDIT_X2, NULL, NULL);
	CreateWindow(text_edit, text_x2, usual,
		rc.right - 150, 50, 30, 20, hWnd, (HMENU)ID_NEDIT_X2, NULL, NULL);
	CreateWindow(text_edit, text_0, usual,
		rc.right - 115, 75, 100, 20, hWnd, (HMENU)ID_EDIT_Y2, NULL, NULL);
	CreateWindow(text_edit, text_y2, usual,
		rc.right - 150, 75, 30, 20, hWnd, (HMENU)ID_NEDIT_Y2, NULL, NULL);

	CreateWindow(text_edit, text_status, usual, rc.right - 300,
		100, 100, 20, hWnd, (HMENU)ID_TEXT_STATUS, NULL, NULL);
	CreateWindow(text_edit, text_sec, usual, rc.right - 300,
		100, 100, 20, hWnd, (HMENU)ID_TEXT_SEC, NULL, NULL);
	CreateWindow(text_edit, text_x, usual, rc.right - 150,
		125, 30, 20, hWnd, (HMENU)ID_NEDIT_XS, NULL, NULL);
	CreateWindow(text_edit, text_y, usual, rc.right - 150,
		125, 30, 20, hWnd, (HMENU)ID_NEDIT_YS, NULL, NULL);
	CreateWindow(text_edit, text_0, usual, rc.right - 150,
		125, 30, 20, hWnd, (HMENU)ID_EDIT_XS, NULL, NULL);
	CreateWindow(text_edit, text_0, usual, rc.right - 150,
		125, 30, 20, hWnd, (HMENU)ID_EDIT_YS, NULL, NULL);

	DWORD style_listview = WS_CHILD | LVS_REPORT | LVS_EX_FULLROWSELECT | WS_VSCROLL | WS_VISIBLE;

	CreateWindow(WC_LISTVIEW, text_0, style_listview, rc.right - 300, 125,
		300, rc.bottom - 100, hWnd, (HMENU)ID_LISTVIEW, NULL, NULL);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
	ListView_SetExtendedListViewStyle(GetDlgItem(hWnd, ID_LISTVIEW), LVS_EX_FULLROWSELECT);

	LV_COLUMN lc; lc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lc.fmt = LVCFMT_CENTER;
	lc.pszText = text_x1; lc.iSubItem = 0; lc.cx = 60;
	SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 0, (LPARAM)&lc);
	lc.pszText = text_y1; lc.iSubItem = 1; lc.cx = 60;
	SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 1, (LPARAM)&lc);
	lc.pszText = text_x2; lc.iSubItem = 2; lc.cx = 60;
	SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 2, (LPARAM)&lc);
	lc.pszText = text_y2; lc.iSubItem = 3; lc.cx = 60;
	SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 3, (LPARAM)&lc);

	SendDlgItemMessage(hWnd, ID_NEDIT_X1, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_NEDIT_Y1, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_NEDIT_X2, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_NEDIT_Y2, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_NEDIT_XS, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_NEDIT_YS, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_TEXT_STATUS, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hWnd, ID_TEXT_SEC, EM_SETREADONLY, 1, 0);
}

// Перерисовка экрана
void moving(HWND hWnd, int with_clean)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	if (with_clean)
	{
		cleanRectOld(hWnd, 0, 0, 300, 0);
	}

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ADD), rc.right - 300, 2, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), rc.right - 200, 2, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_BUILD), rc.right - 300, 22, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CLEAR), rc.right - 200, 22, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), rc.right - 100, 2, 98, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_X1), rc.right - 300, 50, 25, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_X1), rc.right - 270, 50, 110, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_Y1), rc.right - 270, 75, 110, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_Y1), rc.right - 300, 75, 25, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_X2), rc.right - 140, 50, 25, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_X2), rc.right - 110, 50, 110, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_Y2), rc.right - 110, 75, 110, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_Y2), rc.right - 140, 75, 25, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_TEXT_STATUS), rc.right - 450, 2, 130, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_TEXT_SEC), rc.right - 300, 100, 300, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_XS), rc.right - 300, 125, 25, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_XS), rc.right - 270, 125, 110, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_YS), rc.right - 140, 125, 25, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_YS), rc.right - 110, 125, 110, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_LISTVIEW), rc.right - 300, 150, 300, rc.bottom - 100, 0);
}