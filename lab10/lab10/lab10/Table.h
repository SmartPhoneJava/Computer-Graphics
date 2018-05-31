#pragma once
#include "Cut.h"
#include <iostream>

typedef struct Table
{
	Cut* cut;
	Table* next;
} Table;

Table* newTable(Cut *cut);

void deleteTable(Table** t);

Table* searchTable(Table* table, Cut *cut);

Table* addToTable(Table* table, Cut *cut);

bool isCutInTable(Table *A, const Cut &cut);

Table* deleteOfTable(Table* table, Cut *cut);

int getTableSize(Table* table);

void changeTable(Table* table,
	Cut *oldCut, Cut *newCut);

void debugTable(Table* table, const char* text, int number);

Table *lockTable(Table *table);

Table *unlockTable(Table *table);

bool isLock(Table *table);