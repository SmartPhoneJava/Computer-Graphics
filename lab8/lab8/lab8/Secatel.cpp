#include "stdafx.h"
#include "Secatel.h"

Secatel* newSecatel(Point *p)
{
	Secatel *secatel = (Secatel*)calloc(1, sizeof(Secatel));
	secatel->p = p;
	secatel->next = NULL;
	
	return secatel;
}

void pushSecatel(Secatel *secatel, Point *p)
{
	if (!secatel)
		return;

	Secatel* move = secatel;

	while (move->next != NULL)
	{
		move = move->next;
	}
	move->next = newSecatel(p);
}

void deleteSecatel(Secatel **s)
{
	free(*s);
	*s = NULL;
}

void debugSecatel(Secatel *secatel,
	const char* text, int number)
{
	debug(text, number);
	debugPoint(secatel->p, "secatel.point", number);
}

void swap(int &a, int &b)
{
	int c = a;
	a = b;
	b = c;
}