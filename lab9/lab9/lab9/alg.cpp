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
	return signOfNumber(
		cut1->vectorMultiplication(cut2));
}

// Выпуклая ли фигура
int IsConvexFigure(Table *figure)
{
	if (!figure) //Не задан секатель
		return SECATEL_NO;
	if (!figure->next) //В секателе 1 точка
		return SECATEL_IS_ONE_CUT;
	if (!figure->next->next) //В секателе 1 отрезок
		return SECATEL_IS_ONE_CUT;

	// знак векторного произведения
	int prev = sign(
		figure->point,
		figure->next->point,
		figure->next->next->point);
	int curr;

	/*
	Знаки текущего и прошлого векторных
	произведений совпадают
	*/
	bool equal = true;

	Table* move = figure->next;

	/*
	Пока есть ребра и знаки их векторных
	произведений совпадают
	*/
	while (move->next->next && equal)
	{
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

bool isVisible(Point *a, Point *b, Cut *cut)
{
	Cut *c = new Cut(a, b);
	int v = c->vectorMultiplication(*cut);
	return (v > 0);
}

/*
Table *build7(Table* cuts, Table *secatel, int r)
{
	Table* move_cut = cuts;
	Table* move_sec = secatel;
	Table* ret = NULL;
	Point *s = NULL, *f = NULL;

	Cut  *n = NULL, *w = NULL;

	int dsk, wsk;

	int x1, y1, x2, y2;

	double t;

	while(move_cut)
	{
		move_cut = cuts;
		while (move_sec)
		{
			if (move_sec == secatel)
			{
				f = move_sec->cut->getBegin();
			}
			else
			{
				//d = new Cut(move_sec->cut->getBegin(), s);
				
				x1 = 0;
				y1 = 0;

				x2 = -r * move_sec->cut->height();
				y2 = r * move_sec->cut->width();

				n = new Cut(x1, y1, x2, y2);

				dsk = move_cut->cut->scalarMultyplication(*n);
				
				if (dsk != 0)
				{
					w = new Cut(s, move_cut->cut->getBegin());
					wsk = w->scalarMultyplication(*n);

					t = -wsk / dsk;
					if (t >= 0 && t <= 1)
					{
						x1 = s->x + (move_cut->cut->getBX() - s->x) * t;
						y1 = s->y + (move_cut->cut->getBY() - s->y) * t;

						Point *thp = newPoint(x1, y1);
						debugPoint(thp, "addToTablePoint", 0);
						ret = addToTablePoint(ret, thp);
					}
				}
			}
			x1 = 0;
			y1 = 0;

			x2 = -r * move_sec->cut->height();
			y2 = r * move_sec->cut->width();
			n = new Cut(x1, y1, x2, y2);
			s = move_sec->cut->getBegin();

			if (isVisible(s,
				move_cut->cut->getBegin(), n))
			{
				debugPoint(s, "addToTablePoint", 1);
				ret = addToTablePoint(ret, s);
			}

			move_sec = move_sec->next;
		}
		move_cut = move_cut->next;
	}
	return ret;
}
*/
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

	debugPoint(cut->getBegin(), "vis_of_begin", vis1);
	debugPoint(cut->getEnd(), "vis_of_end", vis2);

	if ((vis1 && (!vis2)) || (vis2 && (!vis1)))
	{
		debug("here", 0);
		p2 = cut->getBegin();
		p1 = cut->getEnd();

		q1 = sec->getBegin();
		q2 = sec->getEnd();
		/*
		int a1, a2, b1, b2, c1, c2;

		a1 = cut->width();
		a2 = cut->height();

		b1 = sec->getBX() + sec->getEX();
		b2 = sec->getBY() + sec->getEY();

		c1 = sec->getBX() - cut->getBX();
		c2 = sec->getBY() - cut->getBY();
		*/
		int delta = (p2->x - p1->x) * (q1->y - q2->y) -
			(q1->x - q2->x) * (p2->y - p1->y);

		int	delta_t = (q1->x - p1->x) * (q1->y - q2->y) -
			(q1->x - q2->x) * (q1->y - p1->y);

		if (abs(delta) <= 1e-6)
			return p2;

		double t = (double)delta_t / (double)delta;
		
		/*
		double t = (double)(c1 * b2 - b1 * c2) /
			(double)(a1 * b1 - a2 * b2);
			*/

		Point *ret = newPoint(NO_POINT, NO_POINT);

		ret->x = p1->x + (p2->x - p1->x) * t;
		ret->y = p1->y + (p2->y - p1->y) * t;
		return ret;
	}
	else
		return newPoint(NO_POINT, NO_POINT);
}

//П
Table *build9(Table* cuts, Table *secatel, int r)
{
	Table* move_cut = cuts;
	Table* move_sec = secatel;
	Table* ret = NULL;
	Point *s = NULL, *f = NULL;

	Cut *d = NULL, *n = NULL, *w = NULL;

	int dsk, wsk;

	int x1, y1, x2, y2;

	Point* t;

	int cuts_size = getTableSize(cuts);
	int secatel_size = getTableSize(secatel);

	for (int i = 0; i < secatel_size - 1; i++)
	{
		move_cut = cuts;
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
						move_sec->next->point),
					r);
				if (!isPointFree(t))
				{
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
					move_sec->next->point),
				r);
			if (!isPointFree(t))
			{
				ret = addToTable(ret, t);
			}
		}
		move_sec = move_sec->next;
	}
	/*
	while (move_sec)
	{
		move_cut = cuts;
		while (move_cut)
		{
			if (move_sec == secatel)
			{
				f = move_cut->cut->getBegin();
			}
			else
			{
				t = isIn(new Cut(s, move_cut->cut->getBegin()),
					move_sec->cut, r);
				
				if (!isPointFree(t))
				{	
					debug("add", 0);
					ret = addToTablePoint(ret, t);
				}
			}
			s = move_cut->cut->getBegin();

			if (is_visible(s, move_sec->cut, r))
			{
				debug("add", 1);
				ret = addToTablePoint(ret, s);
			}

			move_cut = move_cut->next;
		}
		if (getTableSize(ret) != 0)
		{
			t = isIn(new Cut(s, f), move_sec->cut, r);
			if (!isPointFree(t))
			{
				debug("add", 1);
				ret = addToTablePoint(ret, t);
			}
		}
		move_sec = move_sec->next;
	}
	*/
	return ret;
}