#pragma once
#include <Windows.h>
#include "Secatel.h"
#include "Point.h"
#include "Alg.h"
#include "Cut.h"
#include "Table.h"

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255, 255, 255)

void drawLine(HDC hdc, int X1, int Y1,
	int X2, int Y2, DWORD color);

void drawCut(HDC hdc, Cut *cut, DWORD color);

void drawCutInside(HDC hdc, Cut *cut,
	Secatel sec, DWORD color);

void drawSecatel(HDC hdc, Secatel sec,
	DWORD color);

void drawEllipse(HDC hdc, int X,
	int Y, int rad, DWORD color);

void drawPoint(HDC hdc, Point *p,
	int rad, DWORD color);

void drawTable(HDC hdc, Table* table, DWORD color);

RECT getRect(HWND hWnd);

void cleanRectOld(HWND hWnd, LONG left,
	LONG top, LONG right, LONG bottom);

void cleanDisplay(HWND hWnd);

void cleanRect(HDC hdc, RECT rect);

void drawPicture(HWND hWnd, Table* table,
	Secatel *sec, DWORD color);

void drawTableInside(HDC hdc, Table *table,
	Secatel sec, DWORD color);