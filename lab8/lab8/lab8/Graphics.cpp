#include "stdafx.h"
#include "Graphics.h"

// Рисование отрезка по двум точкм, состоящим из 2 координат
void drawLine(HDC hdc, int X1, int Y1,
	int X2, int Y2, DWORD color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	
	SelectObject(hdc, hPen);

	MoveToEx(hdc, X1, Y1, NULL);
	LineTo(hdc, X2, Y2);

	DeleteObject(hPen);
}

void drawEllipse(HDC hdc, int X,
	int Y, int rad, DWORD color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);

	SelectObject(hdc, hPen);
	Ellipse(hdc, X - rad, Y - rad, X + rad, Y + rad);
	DeleteObject(hPen);
}

// Рисование точки
void drawPoint(HDC hdc, Point *p,
	int rad, DWORD color)
{
	drawEllipse(hdc, p->x,
		p->y, rad, color);
}

// Рисование двух структур точек
void draw2Points(HDC hdc, Point *p1,
	Point *p2, DWORD color)
{
	debugPoint(p1, "p1", isPointFree(p1));
	debugPoint(p2, "p2", isPointFree(p2));
	if (isPointFree(p1))
		return;
	else if (isPointFree(p2))
		return;
	else
	{
		drawLine(hdc, p1->x, p1->y, p2->x, p2->y, color);
	}
}

// Рисование структуры отрезка
void drawCut(HDC hdc, Cut *cut, DWORD color)
{
	draw2Points(hdc, cut->begin, cut->end, color);
}

// Рисование отрезка внутри секатора
void drawCutInside(HDC hdc, Cut *cut,
	Secatel sec, DWORD color)
{
	debugCut(cut, "cutInside", 1);
	debugCut(cutInside(cut, sec), "cutInside", 0);
	drawCut(hdc, cutInside(cut, sec), color);
}

// Рисование отрезков таблицы внутри секатора
void drawTableInside(HDC hdc, Table *table,
	Secatel sec, DWORD color)
{
	Table* mov = table;
	int size = 0;
	while (mov)
	{
		size++;
		drawCutInside(hdc, mov->cut, sec, color);
		mov = mov->next;
	}
}

// Рисование секатора
void drawSecatel(HDC hdc, Secatel sec, DWORD color)
{
	if (checkSecatel(sec))
	{
		Point *leftUp = newPoint(sec.Xmin, sec.Ymin);
		Point *leftDown = newPoint(sec.Xmin, sec.Ymax);
		Point *rightUp = newPoint(sec.Xmax, sec.Ymin);
		Point *rightDown = newPoint(sec.Xmax, sec.Ymax);

		draw2Points(hdc, leftUp, leftDown, color);
		draw2Points(hdc, rightDown, leftDown, color);
		draw2Points(hdc, rightUp, rightDown, color);
		draw2Points(hdc, leftUp, rightUp, color);
	}
	else
	{
		drawEllipse(hdc, sec.Xmin, sec.Ymin, 3, color);
		drawEllipse(hdc, sec.Xmin, sec.Ymax, 3, color);
		drawEllipse(hdc, sec.Xmax, sec.Ymin, 3, color);
		drawEllipse(hdc, sec.Xmax, sec.Ymax, 3, color);
	}
}

// Отрисовка отрезков
void drawTable(HDC hdc, Table* table, DWORD color)
{
	Table* mov = table;
	while (mov != NULL)
	{
		drawCut(hdc, mov->cut, color);
		mov = mov->next;
	}
}

void drawPicture(HWND hWnd, Table* table,
	Secatel *sec, DWORD color)
{
	HDC hdc = GetDC(hWnd);
	drawTable(hdc, table, color);
	drawSecatel(hdc, *sec, RED);
	ReleaseDC(hWnd, hdc);
}

void cleanRectOld(HWND hWnd, LONG left,
	LONG top, LONG right, LONG bottom)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	if (left > 0)
		rect.left = left;
	if (right > 0)
		rect.right = rect.right - right;
	if (top > 0)
		rect.top = top;
	if (bottom > 0)
		rect.bottom = bottom;

	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	ReleaseDC(hWnd, hdc);
}

RECT getRect(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	return rect;
}

void cleanRect(HDC hdc, RECT rect)
{
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

void cleanDisplay(HWND hWnd)
{
	RECT rect = getRect(hWnd);
	HDC hdc = GetDC(hWnd);
	cleanRect(hdc, rect);
	ReleaseDC(hWnd, hdc);
}