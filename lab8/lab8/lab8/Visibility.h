#pragma once
#include "Debug.h"

#define FULL_UNVISIBLE -1
#define PART_VISIBLE 1
#define FULL_VISIBLE 0

typedef struct Visibility
{
	int T1;
	int T2;
	int T3;
	int T4;
} Visibility;

int isPointVisible(Visibility vis);

Visibility* newVisibility(int t1,
	int t2, int t3, int t4);

void deleteVisibility(Visibility **v);

// Степень видимости отрезка(полная видимость\невидимость, частичная)
int checkCut(Visibility vis1, Visibility vis2);

void debugVisibility(Visibility visibility,
	const char* text, int number);