#pragma once
#include "Visibility.h"
#include "Secatel.h"
#include "Debug.h"
#include "Resource.h"

typedef struct Point
{
	int x;
	int y;
	Visibility vis;
} Point;

Point* newPoint(int x, int y);

void deletePoint(Point **point);

void setPointVisibility(Point &p, Secatel s);

bool isPointFree(Point *point);

void debugPoint(Point *point, const char* text, int number);

bool comparePoints(Point *A, Point *B);