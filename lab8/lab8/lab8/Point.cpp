#include "stdafx.h"
#include "Point.h"
#include "Secatel.h"
#include <iostream>

Point* newPoint(int x, int y)
{
	Point* point = (Point*)calloc(1, sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

void deletePoint(Point **point)
{
	free(*point);
	*point = NULL;
}

bool isPointFree(Point *point)
{
	bool x = (point->x == NO_POINT);
	bool y = (point->y == NO_POINT);
	return (x && y);
}

void setPointVisibility(Point &p, Secatel s)
{
	..
}

void debugPoint(Point *point, const char* text, int number)
{
	debug(text, number);
	debug("x_point", point->x);
	debug("y_point", point->y);
}

bool comparePoints(Point *A, Point *B)
{
	bool x = (A->x == B->x);
	bool y = (A->y == B->y);
	return (x && y);
}