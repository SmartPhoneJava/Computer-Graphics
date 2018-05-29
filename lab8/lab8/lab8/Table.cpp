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
		if (isCutInTable(mov, *cut))
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

bool isCutInTable(Table *A, const Cut &cut)
{
	return (A->cut)->compareWithCut(cut);
}

Table* deleteOfTable(Table* table, Cut *cut)
{
	if (!table)
		return NULL;
	
	if (isCutInTable(table, *cut))
	{
		return deleteTableAndGetNext(&table);
	}

	Table* mov = table;
	while (mov->next != NULL)
	{
		if (isCutInTable(mov->next, *cut))
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
		(mov->cut)->debugCut("cut(Table) ", count);
		mov = mov->next;
	}
}

Table *getLast(Table *table)
{
	Table* move = table;
	while (move->next)
	{
		move = move->next;
	}
	return move;
}

bool isLock(Table *table)
{
	return (table->cut->getBegin() ==
		getLast(table)->cut->getEnd());
}

Table *lockTable(Table *table)
{
	if (!table || isLock(table))
		return table;

	return addToTable(table, new Cut(
		getLast(table)->cut->getEnd(),
		table->cut->getBegin())
	);
}

Table* deleteLast(Table* table)
{
	if (!table)
		return NULL;

	Table* prev = table;
	Table* move = table;
	while (move->next)
	{
		prev = move;
		move = move->next;
	}
	prev->next = NULL;


	return table; //deleteOfTable(table, getLast(table)->cut);
}

Table* unlockTable(Table *table)
{
	if (!table)
		return table;

	return deleteLast(table);
}
