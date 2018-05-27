#pragma once
#include "Debug.h"
#include "Point.h"

typedef struct Secatel
{
	Point *p;
	Point *n;
	Secatel* next;
} Secatel;

Secatel *newSecatel(Point *p);

void pushSecatel(Secatel *secatel, Point *p);

void popSecatel(Secatel *secatel, Point *p);

void deleteSecatel(Secatel **s);

void debugSecatel(Secatel *secatel,
	const char* text, int number);