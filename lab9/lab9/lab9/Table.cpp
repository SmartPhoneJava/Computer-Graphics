#include "stdafx.h"
#include "Table.h"

Table* newTable(Point *point)
{
	Table* table = (Table*)calloc(1, sizeof(Table));
	table->point = point;
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

Table* searchTable(Table* table, Point *Point)
{
	Table* mov = table;

	while (mov != NULL)
	{
		if (isPointInTable(mov, Point))
			break;
		mov = mov->next;
	}
	return mov;
}

Table* addToTable(Table* table, Point *point)
{
	if (!table)
	{
		return newTable(point);
	}

	Table* mov = table;

	while (mov->next != NULL)
	{
		mov = mov->next;
	}

	mov->next = newTable(point);

	return table;
}

bool isPointInTable(Table *A, Point *point)
{
	return (A->point == point);
}

Table* deleteOfTable(Table* table, Point *point)
{
	if (!table)
		return NULL;
	
	if (isPointInTable(table, point))
	{
		return deleteTableAndGetNext(&table);
	}

	Table* mov = table;
	while (mov->next != NULL)
	{
		if (isPointInTable(mov->next, point))
			break;
			
		mov = mov->next;
	}
	if (mov->next)
		mov->next = deleteTableAndGetNext(&(mov->next));
	
	return table;
}

void changeTable(Table* table,
	Point *oldPoint, Point *newPoint)
{
	Table *tab = searchTable(table, oldPoint);
	if (tab)
	{
		tab->point = newPoint;
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
		debugPoint(mov->point, "cut(Table) ", count);
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
	if (!table)
		return false;
	return (table->point == getLast(table)->point);
}

Table *lockTable(Table *table)
{
	if (!table || isLock(table))
		return table;

	return addToTable(table, table->point);
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
	if (!table || !isLock(table))
		return table;

	return deleteLast(table);
}
