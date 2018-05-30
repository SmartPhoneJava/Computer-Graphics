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

int sign(Cut &a, Cut &b)
{
	return signOfNumber(a.vectorMultiplication(b));
}

// Выпуклая ли фигура
int IsConvexFigure(Table *figure)
{
	if (!figure) //Не задан секатель
		return SECATEL_NO;
	if (!figure->next) //В секателе 1 отрезок
		return SECATEL_IS_ONE_CUT;

	// знак векторного произведения
	int prev = sign(
		*figure->cut,
		*figure->next->cut);
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
	while (move->next && equal)
	{
		curr = sign(
			*move->cut,
			*move->next->cut);
		if (curr * prev == -1)
			equal = false;
		prev = curr;

		move = move->next;
	}
	return equal * curr;
}

Cut *build(Cut* cut, Table *secatel, int r)
{
	// нижнее и верхнее t
	double tb = 0, te = 1, t;

	Table *move = secatel;
	Cut *w = new Cut();
	Cut n;

	int x1, y1, x2, y2;

	int dscalar, wscalar;

	while (move)
	{
		/* весовой множитель удаленности
		гранничной точки от р1(берем граничную
		точку равной вершине)
		*/
		w = new Cut(move->cut->getBegin(),
			cut->getBegin());

		x1 = 0;
		y1 = 0;

		x2 = -r * move->cut->height();
		y2 = r * move->cut->width();

		n = Cut(x1, y1, x2, y2);

		dscalar = cut->scalarMultyplication(n);
		wscalar = w->scalarMultyplication(n);

		// Если отрезок параллелен ребру отсекателя
		if (dscalar == 0)
		{ // Отрезок невидим
			if (wscalar < 0)
			{
				return new Cut();
			}
			// else continue
		}
		else
		{ // отрезок не вырожден, определяем t
			t = (double)-wscalar / (double)dscalar;

			//Поиск верхнего и нижнего предела t

			if (dscalar > 0)
			{ // поиск нижнего
				if (t > 1)
				{ // начало видимой части за
					return new Cut();
				} // отсекателем
				else
				{
					tb = max(tb, t);
				}
			}
			else
			{ // поиск верхнего
				if (t < 0)
				{
					return new Cut();
				}
				else
				{
					te = min(te, t);
				}
			}
		}
		move = move->next;
	}
	// Проверка фактической видимости отрезка
	if (tb <= te)
	{
		x1 = cut->getBX() + (cut->getEX() - cut->getBX()) * tb;
		y1 = cut->getBY() + (cut->getEY() - cut->getBY()) * tb;

		x2 = cut->getBX() + (cut->getEX() - cut->getBX()) * te;
		y2 = cut->getBY() + (cut->getEY() - cut->getBY()) * te;
		return new Cut(x1, y1, x2, y2);
	}
	return new Cut();
}

/////////////////////////////////////////

bool isVisible(Point *a, Point *b, Cut *cut)
{
	Cut *c = new Cut(a, b);
	int v = c->vectorMultiplication(*cut);
	return (v > 0);
}


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

bool is_visible(Point *p, Cut* sec, int r)
{
	Cut *a = new Cut(sec->getBegin(), p);
	int vec = a->vectorMultiplication(*sec);
	return (r * vec < 0);
}

Point *isIn(Cut* cut, Cut* sec, int r)
{
	bool vis1 = is_visible(cut->getBegin(), sec, r);
	bool vis2 = is_visible(cut->getEnd(), sec, r);

	Point *p1, *p2, *q1, *q2;

	if ((vis1 && (!vis2)) || (vis2 && (!vis1)))
	{
		p1 = cut->getBegin();
		p2 = cut->getEnd();

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

//П
Table *build9(Table* cuts, Table *secatel, int r)
{
	Table* move_cut = cuts;
	Table* move_sec = NULL;
	Table* ret = NULL;
	Point *s = NULL, *f = NULL;

	Cut *d = NULL, *n = NULL, *w = NULL;

	int dsk, wsk;

	int x1, y1, x2, y2;

	Point* t;

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
	return ret;
}