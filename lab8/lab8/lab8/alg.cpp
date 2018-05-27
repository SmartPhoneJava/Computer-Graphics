#include "stdafx.h"
#include "Alg.h"

// ������� �������� ����� �����������
int count(double tangens, int coord_secator,
	int coord1_point, int coord2_point)
{
	int min = coord_secator - coord1_point;
	return tangens * min + coord2_point;
}

/*
������� ������. ����� �� ������ ������� 
�����/������/����/���� ��������
T1 - �����
T2 - ������
T3 - ����(y ������)
T4 - ����(y ����)
����� �����. ���� �� ���� �� ��������, �� NULL
*/

// ����� �� ������ ������� ����� ��������
Point* whoLeft(Cut *cut)
{
	if (cut->begin->vis.T1 == 1)
		return cut->begin;
	if (cut->end->vis.T1 == 1)
		return cut->end;
	return NULL;
}

// ����� �� ������ ������� ������ ��������
Point* whoRight(Cut *cut)
{
	if (cut->begin->vis.T2 == 1)
		return cut->begin;
	if (cut->end->vis.T2 == 1)
		return cut->end;
	return NULL;
}

// ����� �� ������ ������� ���� ��������
Point* whoUp(Cut *cut)
{
	if (cut->begin->vis.T3 == 1)
		return cut->begin;
	if (cut->end->vis.T3 == 1)
		return cut->end;
	return NULL;
}

// ����� �� ������ ������� ���� ��������
Point* whoDown(Cut *cut)
{
	if (cut->begin->vis.T4 == 1)
		return cut->begin;
	if (cut->end->vis.T4 == 1)
		return cut->end;
	return NULL;
}

/*
������� ��������
*/

// a <= b <= c
bool checkBorder(int a, int b, int c)
{
	return ((a <= b) && (b <= c));
}

// ���������, ��� x ����� ������ ��������
bool checkHorizontal(Secatel secatel, int b)
{
	int a = secatel.Xmin;
	int c = secatel.Xmax;
	if (a < c)
		return checkBorder(a, b, c);
	else
		return checkBorder(c, b, a);
}

// ���������, ��� y ����� ������ ��������
bool checkVertical(Secatel secatel, int b)
{
	int a = secatel.Ymin;
	int c = secatel.Ymax;
	if (a < c)
		return checkBorder(a, b, c);
	else
		return checkBorder(c, b, a);
}

// ���������, ��� ����� ����� ������ ��������
bool checkPoint(Secatel sec, Point point)
{
	bool first = checkHorizontal(sec, point.x);
	bool second = checkVertical(sec, point.y);
	return (first && second);
}

/*
������� ������ ����� ����������� � ���������
����� ����� �����������, ���� NULL, ���� �� ���
*/

// ����� ����������� � ����� ������ ��������
Point* borderLeft(Point *p, Secatel sec, double m)
{
	int x_new = sec.Xmin;
	int y_new = NO_POINT;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		y_new = p->y;
	else
	{
		y_new = count(m, x_new, p->x, p->y);
		return newPoint(x_new, y_new);
	}

	return newPoint(x_new, y_new);
}

// ����� ����������� � ������ ������ ��������
Point* borderRight(Point *p, Secatel sec, double m)
{
	int x_new = sec.Xmax;
	int y_new = NO_POINT;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		y_new = p->y;
	else
		y_new = count(m, x_new, p->x, p->y);
	return newPoint(x_new, y_new);
}

// ����� ����������� � ������� ������ ��������
Point* borderUp(Point *p, Secatel sec, double m)
{
	int x_new = NO_POINT;
	int y_new = sec.Ymin;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		x_new = p->x;
	else
		x_new = count(1 / m, y_new, p->y, p->x);
	return newPoint(x_new, y_new);
}

// ����� ����������� � ������ ������ ��������
Point* borderDown(Point *p, Secatel sec, double m)
{
	int x_new = NO_POINT;
	int y_new = sec.Ymax;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		x_new = p->x;
	else 
		x_new = count(1 / m, y_new, p->y, p->x);
	return newPoint(x_new, y_new);
}

// ��������� �� ������� ������ ����������� �����
// �� ����� ������� � ����� ������� ���������
typedef Point*(*Who)(Cut *cut);

// ��������� �� ������� ������ ����� �����������
typedef Point*(*Border)(Point *p,
	Secatel sec, double m);

// ��������� � ������(������� ����������� �������)
// �� ����� �������� 
Point* move(Border border, Who who, Cut *cut,
	Secatel sec, double tg)
{
	if (!who(cut))
		return NULL;
	Point *point = newPoint(who(cut)->x, who(cut)->y);
	Point *result = NULL;
	if (point)
		result = border(point, sec, tg);
	if (result)
		debugPoint(result, "result in move", 0);
	return result;
	
}

void setIfFree(Secatel secatel, Point *from,
	Point* to1, Point *to2)
{
	if (!from)
		return;
	
	if (!checkPoint(secatel, *from))
	{
		return;
	}
	
	if (isPointFree(to1))
	{
		to1->x = from->x;
		to1->y = from->y;
	}
	else if (isPointFree(to2))
	{
		to2->x = from->x;
		to2->y = from->y;
	}
}

bool nearAB(int a, int b)
{
	return ((b - 5 < a) && (b + 5 > a));
}

void setIfBool(int &a, int set, bool b)
{
	if (b)
	{
		a = set;
	}
	
}

// ��������� �������
Cut* cutInside(Cut *cut, Secatel *secatel)
{
	int dx = getCutDX(cut);
	int dy = getCutDY(cut);

	Point *d, *w;

	d = newPoint(dx, dy);

	Secatel *move = secatel;

	Cut *rebro = newCutInt(NO_POINT, NO_POINT, NO_POINT, NO_POINT);

	Cut *vektor;

	while (move->next != NULL)
	{
		//deleteCut(&cut);
		rebro = newCut(move->p, move->next->p);
		vektor = newCut(rebro->begin, cut->begin);

		dx = getCutDX(vektor);

		w = newPoint(dx, dy);

	}

	Point *d = newPoint(dx, dy);
}

