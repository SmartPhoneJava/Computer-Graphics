#pragma once
#include "Debug.h"

typedef struct Secatel
{
	int Ymin;
	int Ymax;
	int Xmin;
	int Xmax;
} Secatel;

Secatel *newSecatel(int up, int right, int down, int left);

void deleteSecatel(Secatel **s);

void debugSecatel(Secatel secatel,
	const char* text, int number);

bool checkSecatel(Secatel sec);

void makeNullSecatel(Secatel &sec);

void fixSecatel(Secatel &sec);