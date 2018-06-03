#pragma once
#include <windows.h>
#include <iostream>
#include <CommCtrl.h>
#include "Resource.h"
#include "Graphics.h"
#include "Alg.h"
#include "Table.h"

#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4

#define LEFT_TOP 5
#define RIGHT_TOP 6
#define LEFT_BOTTOM 7
#define RIGHT_BOTTOM 8

#define NO_NEED -1


void setCharText(HWND hwnd, const char* text);

void setIntText(HWND hWnd, int ID, int d);

void getMouseCoordinates(LPARAM lParam,
	int borderX, int &x, int &y);

void setMouseCoordinates(HWND hWnd, int x,
	int y, int ID1, int ID2);

void deleteListViewItem(HWND listview, int i);

/*
Table* adding(HWND hWnd, Table *table);

void updatingListView(HWND hWnd, LPARAM lParam,
	int &listview_choosen, int &x1, int &y1,
	int &x2, int &y2, Table* lines, Table *secatel);

void deleting(HWND hWnd, int listview_choosen,
	int x1, int y1, int x2, int y2, Table* table);

void changing(HWND hWnd, int listview_choosen,
	Table* table, int old_x1, int old_y1,
	int old_x2, int old_y2);
	*/
void creating(HWND hWnd);

void moving(HWND hWnd, int with_clean);