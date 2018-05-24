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
	int t1, t2, t3, t4;
	if (p.x < s.Xmin)  
		t1 = 1;
	else
		t1 = 0;

	if (p.x > s.Xmax)  
		t2 = 1;
	else
		t2 = 0;

	if (p.y < s.Ymin)
		t3 = 1;
	else
		t3 = 0;

	if (p.y > s.Ymax)  
		t4 = 1;
	else
		t4 = 0;

	p.vis = *newVisibility(t1, t2, t3, t4);
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