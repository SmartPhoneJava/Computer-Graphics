#include "stdafx.h"
#include "Secatel.h"

Secatel* newSecatel(int up, int right, int down, int left)
{
	Secatel *secatel = (Secatel*)calloc(1, sizeof(Secatel));
	secatel->Xmax = right;
	secatel->Xmin = left;
	secatel->Ymax = down;
	secatel->Ymin = up;
	
	return secatel;
}

void deleteSecatel(Secatel **s)
{
	free(*s);
	*s = NULL;
}

void debugSecatel(Secatel secatel,
	const char* text, int number)
{
	debug(text, number);
	debug("secatel.Xmax", secatel.Xmax);
	debug("secatel.Xmin", secatel.Xmin);
	debug("secatel.Ymin", secatel.Ymin);
	debug("secatel.Ymax", secatel.Ymax);
}

void makeNullSecatel(Secatel &sec)
{
	sec.Xmax = NO_POINT;
	sec.Xmin = NO_POINT;
	sec.Ymax = NO_POINT;
	sec.Ymin = NO_POINT;
}

bool checkSecatel(Secatel sec)
{
	bool b1 = (sec.Xmax != NO_POINT);
	bool b2 = (sec.Xmin != NO_POINT);
	bool b3 = (sec.Ymax != NO_POINT);
	bool b4 = (sec.Ymin != NO_POINT);

	return (b1 && b2 && b3 && b4);
}