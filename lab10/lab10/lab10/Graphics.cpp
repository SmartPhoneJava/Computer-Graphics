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
	drawEllipse(hdc, p->getX(),
		p->getY(), rad, color);
}

// Рисование двух структур точек
void draw2Points(HDC hdc, Point *p1,
	Point *p2, DWORD color)
{
	if (p1->isFree())
		return;
	else if (p2->isFree())
		return;
	else
	{
		drawLine(hdc, p1->getX(), p1->getY(),
			p2->getX(), p2->getY(), color);
	}
}

// Рисование структуры отрезка
void drawCut(HDC hdc, Cut cut, DWORD color)
{
	draw2Points(hdc, cut.getBegin(), cut.getEnd(), color);
}

/*
// Рисование отрезков таблицы внутри секатора
int drawTableInside(HDC hdc, Table *table,
	Table* secatel, DWORD color)
{
	if (!table && !secatel)
		return NO_CUTS_AND_SECATEL;

	if (!table)
		return NO_CUTS;

	Table* mov = table;
	
	int r = IsConvexFigure(secatel);
	if (r != SECATEL_INSIDE_RIGHT && r != SECATEL_INSIDE_LEFT)
		return SECATEL_ISNT_CONVEX;

	Cut *cut;

	while (mov)
	{
		cut = build(mov->cut, secatel, r);
		drawCut(hdc, *cut, color);
		mov = mov->next;
	}
	drawTable(hdc, secatel, RGB(255, 0, 0));
	return NOERROR;
}
*/

// Отрисовка отрезка или списка рёбер
void drawTable(HDC hdc, Table* table, DWORD color)
{
	Table* mov = table;
	Cut *cut = new Cut();
	while (mov != NULL)
	{
		cut = mov->cut;
		drawCut(hdc, *cut, color);
		mov = mov->next;
	}
}

void drawPicture(HWND hWnd, Table* lines,
	Table *secatel, DWORD color)
{
	HDC hdc = GetDC(hWnd);
	drawTable(hdc, lines, color);
	drawTable(hdc, secatel, RED);
	ReleaseDC(hWnd, hdc);
}

void cleanRectOld(HWND hWnd, LONG left,
	LONG top, LONG right, LONG bottom)
{
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