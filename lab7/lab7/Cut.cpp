#include "stdafx.h"
#include "Cut.h"
#include <iostream>

int getCutDX(Cut *cut)
{
	return cut->end->x - cut->begin->x;
}

int getCutDY(Cut *cut)
{
	return cut->end->y - cut->begin->y;
}

Cut* newCut(Point *a, Point *b)
{
	Cut *cut = (Cut*)calloc(1, sizeof(Cut));
	cut->begin = a;
	cut->end = b;
	return cut;
}

Cut* newCutInt(int x1, int y1, int x2, int y2)
{
	return newCut(newPoint(x1, y1), newPoint(x2, y2));
}

void deleteCut(Cut **cut)
{
	free(*cut);
	*cut = NULL;
}

double cutTan(Cut *cut)
{
	int dx = getCutDX(cut);
	int dy = getCutDY(cut);

	//debug("dx", dx);
	//debug("dy", dy);

	if (dx == 0)
		return VERTICAL;
	else if (dy == 0)
		return HORIZONTAL;
	else
	{
		double result = (double)dy / (double)dx;
		//debugDouble("return_tg", result);
		return result;
	}
}

void debugCut(Cut *cut, const char* text, int number)
{
	debug(text, number);
	debug("x1_cut", cut->begin->x);
	debug("y1_cut", cut->begin->y);
	debug("x2_cut", cut->end->x);
	debug("y2_cut", cut->end->y);
}

void debugCutVisibility(Cut *cut, const char* text, int number)
{
	debug(text, number);

	Visibility v1 = cut->begin->vis;
	Visibility v2 = cut->end->vis;

	debugVisibility(v1, "Visibility of first point",
		isPointVisible(v1));
	debugVisibility(v2, "Visibility of second point",
		isPointVisible(v2));
}

bool compareCuts(Cut* A, Cut *B)
{
	bool begin = comparePoints(A->begin, B->begin);
	bool end = comparePoints(A->end, B->end);
	return (begin && end);
}