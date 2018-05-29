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