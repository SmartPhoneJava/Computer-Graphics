#pragma once
#include "Debug.h"
#include "Resource.h"

typedef struct Point
{
	int x;
	int y;
} Point;

Point* newPoint(int x, int y);

void deletePoint(Point **point);

bool isPointFree(Point *point);

void debugPoint(Point *point, const char* text, int number);

bool comparePoints(Point *A, Point *B);