#include "stdafx.h"
#include "Alg.h"

int signOfNumber(const int &a)
{
	if (a < 0)
		return -1;
	else if (a > 0)
		return 1;
	return 0;
}

int sign(Point *a, Point *b, Point *c)
{
	Cut *cut1 = new Cut(a, b);
	Cut *cut2 = new Cut(b, c);
	int number = signOfNumber(
		cut1->vectorMultiplication(cut2));
	debug("number", number);
	return number;
	//delete cut1;
	//delete cut2;
	//return number;
}

// ¬ыпукла€ ли фигура
int IsConvexFigure(Table *figure)
{
	if (!figure) //Ќе задан секатель
		return SECATEL_NO;
	if (!figure->next) //¬ секателе 1 точка
		return SECATEL_IS_ONE_POINT;
	if (!figure->next->next) //¬ секателе 1 отрезок
		return SECATEL_IS_ONE_CUT;

	// знак векторного произведени€
	int prev = sign(
		figure->point,
		figure->next->point,
		figure->next->next->point);
	int curr;

	/*
	«наки текущего и прошлого векторных
	произведений совпадают
	*/
	bool equal = true;

	Table* move = figure->next;

	/*
	ѕока есть ребра и знаки их векторных
	произведений совпадают
	*/
	int i = 0;
	while (move->next->next && equal)
	{
		i++;
		debug("one", i);
		curr = sign(
			figure->point,
			figure->next->point,
			figure->next->next->point);
		if (curr * prev == -1)
			equal = false;
		prev = curr;

		move = move->next;
	}
	return equal * curr;
}

bool is_visible(Point *p, Point* p1, Point *p2, int r)
{
	Cut *a = new Cut(p1, p);
	Cut *b = new Cut(p1, p2);
	int vec = a->vectorMultiplication(b);
	return (r * vec < 0);
}

Point *isIn(Cut* cut, Cut* sec, int r)
{
	bool vis1 = is_visible(cut->getBegin(),
		sec->getBegin(), sec->getEnd(), r);
	bool vis2 = is_visible(cut->getEnd(),
		sec->getBegin(), sec->getEnd(), r);

	Point *p1, *p2, *q1, *q2;

	if ((vis1 && (!vis2)) || (vis2 && (!vis1)))
	{
		p2 = cut->getBegin();
		p1 = cut->getEnd();

		q1 = sec->getBegin();
		q2 = sec->getEnd();
	
		int delta = (p2->x - p1->x) * (q1->y - q2->y) -
			(q1->x - q2->x) * (p2->y - p1->y);

		int	delta_t = (q1->x - p1->x) * (q1->y - q2->y) -
			(q1->x - q2->x) * (q1->y - p1->y);

		if (abs(delta) <= 1e-6)
			return p2;

		double t = (double)delta_t / (double)delta;

		Point *ret = newPoint(NO_POINT, NO_POINT);

		ret->x = p1->x + (p2->x - p1->x) * t;
		ret->y = p1->y + (p2->y - p1->y) * t;
		return ret;
	}
	else
		return newPoint(NO_POINT, NO_POINT);
}

Table *build9(HDC hdc, Table* cuts, Table *secatel, int r)
{
	Table* move_cut = cuts;
	Table* move_sec = secatel;
	Table* ret = NULL;
	Point *s = NULL, *f = NULL;

	Point* t;

	int cuts_size;
	int secatel_size = getTableSize(secatel);

	for (int i = 0; i < secatel_size - 1; i++)
	{
		ret = NULL;
		cuts_size = getTableSize(move_cut);
		for (int j = 0; j < cuts_size; j++)
		{
			if (j == 0)
			{
				f = move_cut->point;
			}
			else
			{
				t = isIn(new Cut(s, move_cut->point),
					new Cut(move_sec->point,
						move_sec->next->point), r);
				if (!isPointFree(t))
				{
					debugPoint(t, "t1", 0);
					ret = addToTable(ret, t);
				}
			}
			s = move_cut->point;
			if (is_visible(s, move_sec->point,
				move_sec->next->point, r))
			{
				ret = addToTable(ret, s);
			}
			move_cut = move_cut->next;
		}
		if (getTableSize(ret) != 0)
		{
			t = isIn(new Cut(s, f),
				new Cut(move_sec->point,
					move_sec->next->point), r);
			if (!isPointFree(t))
			{
				debugPoint(t, "t2", 0);
				ret = addToTable(ret, t);
			}
		}
		move_cut = ret;
		move_sec = move_sec->next;
	}

	if (getTableSize(move_cut) != 0)
	{
		while (move_cut->next)
		{
			drawCut(hdc, *(new Cut(move_cut->point,
				move_cut->next->point)), RGB(0, 255, 0));
			move_cut = move_cut->next;
		}
	}
	return ret;
}