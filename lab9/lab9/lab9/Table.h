#pragma once
#include "Cut.h"
#include <iostream>

typedef struct Table
{
	Point* point;
	Table* next;
} Table;

Table* newTable(Cut *cut);

void deleteTable(Table** t);

Table* searchTable(Table* table, Point *p);

Table* addToTable(Table* table, Point *p);

bool isPointInTable(Table *A, Point *p);

Table* deleteOfTable(Table* table, Point *p);

int getTableSize(Table* table);

void changeTable(Table* table,
	Point *oldPoint, Point *newPoint);

void debugTable(Table* table, const char* text, int number);

Table* deleteLast(Table* table);

//Table *lockTable(Table *table);

//Table *unlockTable(Table *table);

//bool isLock(Table *table);