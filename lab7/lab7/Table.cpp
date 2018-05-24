#include "stdafx.h"
#include "Table.h"

Table* newTable(Cut *cut)
{
	Table* table = (Table*)calloc(1, sizeof(Table));
	table->cut = cut;
	table->next = NULL;
	return table;
}

void deleteTable(Table** t)
{
	free(*t);
	*t = NULL;
}

Table* deleteTableAndGetNext(Table** t)
{
	Table *ret = NULL;
	if (!(*t) && !((*t)->next))
		ret = (*t)->next;

	deleteTable(t);
	return ret;
}

Table* searchTable(Table* table, Cut *cut)
{
	Table* mov = table;

	while (mov != NULL)
	{
		if (isCutInTable(mov, cut))
			break;
		mov = mov->next;
	}
	return mov;
}

Table* addToTable(Table* table, Cut *cut)
{
	if (!table)
	{
		return newTable(cut);
	}

	Table* mov = table;

	while (mov->next != NULL)
	{
		mov = mov->next;
	}

	mov->next = newTable(cut);

	return table;
}

bool isCutInTable(Table *A, Cut *cut)
{
	return compareCuts(A->cut, cut);
}

Table* deleteOfTable(Table* table, Cut *cut)
{
	if (!table)
		return NULL;
	
	if (isCutInTable(table, cut))
	{
		return deleteTableAndGetNext(&table);
	}

	Table* mov = table;
	while (mov->next != NULL)
	{
		if (isCutInTable(mov->next, cut))
			break;
			
		mov = mov->next;
	}
	if (mov->next)
		mov->next = deleteTableAndGetNext(&(mov->next));
	
	return table;
}

void changeTable(Table* table, Cut *oldCut, Cut *newCut)
{
	Table *tab = searchTable(table, oldCut);
	if (tab)
	{
		tab->cut = newCut;
	}
}

int getTableSize(Table* table)
{
	int count = 0;
	Table* mov = table;

	while (mov != NULL)
	{
		count++;
		mov = mov->next;
	}
	return count;
}

void debugTable(Table* table, const char* text, int number)
{
	debug(text, number);
	Table* mov = table;
	if (mov)
		debug("table not NULL", 1);

	int count = 0;
	while (mov != NULL)
	{
		count++;
		debugCut(mov->cut, "cut(Table) ", count);
		mov = mov->next;
	}
}