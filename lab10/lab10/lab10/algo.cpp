#include "stdafx.h"
#include "algo.h"

void FindYBroders(Table *table, double &ymax, double &ymin)
{
	ymax = ymin = table->cut->getBegin()->getY();
	Table* move = table;
	double y_end, y_begin;
	int i = 0;
	while (move)
	{
		y_begin = move->cut->getBegin()->getY();
		y_end = move->cut->getEnd()->getY();
		debug("y_begin", y_begin);
		debug("y_end", y_end);

		if (y_begin > ymax)
			ymax = y_begin;
		if (y_end > ymax)
			ymax = y_end;
		if (y_begin < ymin)
			ymin = y_begin;
		if (y_end < ymin)
			ymin = y_end;
		move = move->next;
	}
}

void MovingGraphic(Table *table, double dx, double dy)
{
	Table* move = table;
	while (move)
	{
		move->cut->setBX(move->cut->getBX() + dx);
		move->cut->setBY(move->cut->getBY() + dy);

		move->cut->setEX(move->cut->getEX() + dx);
		move->cut->setEY(move->cut->getEY() + dy);

		move = move->next;
	}
}

int VisiblePoint(Point &low, Point &high, Point &tmp)
{
    if(tmp.getY() > high.getY())
        return 1;
    if(tmp.getY() < low.getY())
        return -1;
    return 0;
}


Point* IntersectionSegments(Point &p1, Point &p2,
	Point &p3, Point &p4, double z)
{
    double k1 = (p2.getY() - p1.getY()) /
		(p2.getX() - p1.getX());
    double b1 = (p2.getX() * p1.getY() -
		p1.getX() * p2.getY()) / (p2.getX() - p1.getX());
    double k2 = (p4.getY() - p3.getY()) /
		(p4.getX() - p3.getX());
    double b2 = (p4.getX() * p3.getY() - p3.getX() *
		p4.getY()) / (p4.getX() - p3.getX());
    
	double x = (b2 - b1) / (k1 - k2);
	double y = k1 * x + b1;
    return new Point(x, y, z);
}
double Round(double x) //magic
{
    x *= 100000;
    x = round(x);
    x /= 100000;
    return x;
}

void cutApply(HDC hdc, Table **table, Point* p1, Point* p2)
{
	p1->addX(100);
	p2->addX(100);

	Scale scale{ 2, 2, 1};
	t_matrix a;
	scalingMatrix(a, scale);

	p1->swap();
	p2->swap();

	//p1->update(a);

	//MovingGraphic(*table, 500, 200);
	Cut* cut = new Cut(p1, p2);

	*table = addToTable(*table, cut);
	drawCut(hdc, *cut, RGB(255, 0, 0));
}

void GorisontAlgo_new(HDC hdc, tFunction &func,
	Table** table)
{
	vector<Point> high;
	vector<Point> low;

	/*
	Инициализация первого плавающего горизонта
	(нижнего и верхнего)
	*/
	double X, Y, Z = func.zmin;
	double xMax = func.xmax, xMin = func.xmin, dx = func.dx;

	debugDouble("func.f(x, Z)", func.f(5, 100));
	for (double x = xMin; x < xMax; x += dx)
	{
		Y = func.f(x, Z);
		high.push_back(Point(x, Y, Z));
		low.push_back(Point(x, Y, Z));
	}

	/*
	Обновление горизонтов
	*/

	double dz = (double)(func.zmax - func.zmin) /
		(func.n - 1);
	double zMin = func.zmin;
	double zMax = func.zmax + EPS;

	double mMin = low[0].getY();
	double mMax = low[0].getY();

	// I - точка пересения
	Point *pointPrev, *pointCurr, *I;

	int visPrev, visCurr;

	int pointCount = low.size();

	Point *firstPrev = NULL, *firstCurr = NULL;
	Point *lastPrev = NULL, *lastCurr = NULL;

	bool FIRST_TIME = true;

	double xLast, xFirst;

	Cut *cut;

	// Идём в глубину
	for (double z = zMin; z <= zMax; z += dz)
	{
		if (z > zMax)
			z = zMax;

		{
			xFirst = low[0].getX();
			firstCurr = new Point(xFirst,
				func.f(xFirst, z), z);
			if (!FIRST_TIME)
			{
				//cut = new Cut(firstPrev, firstCurr);
				//*table = addToTable(*table, cut);
				//drawCut(hdc, *cut, RGB(255, 0, 0));
				cutApply(hdc, table, firstPrev, firstCurr);
			}
			firstPrev = firstCurr;
		}


		lastCurr = new Point(
			low[pointCount - 1].getX(),
			func.f(low[pointCount - 1].getX(), z), z);

		if (!FIRST_TIME)
		{
			//*table = addToTable(*table, new Cut(lastPrev, lastCurr));
			cutApply(hdc, table, lastPrev, lastCurr);
		}

		lastPrev = lastCurr;

		FIRST_TIME = false;

		/* 
		Первая точка, нужна, чтобы дальнейшая
		точка имела значение видимости предыдущей
		*/
		pointPrev = new Point(xMin, func.f(xMin, z), z);
		visPrev = pointPrev->isVisible(low[0], high[0]);
		for (int j = 1; j < pointCount; j++)
		{
			X = low[j].getX();
			pointCurr = new Point(X, func.f(X, z), z);
			
			visCurr = pointCurr->isVisible(low[j], high[j]);
			if (visCurr == visPrev) 
			{ // Если оба видимы или не видимы
				if (visCurr) // то в случае видимости отрисуем
				{
					//*table = addToTable(*table, new Cut(pointPrev, pointCurr));
					cutApply(hdc, table, pointPrev, pointCurr);
				}
			}
			else if (visCurr && visPrev)
			{ // Если одна точка видима, а вторая нет
				I = IntersectionSegments(*pointPrev,
					*pointCurr, low[j - 1], low[j], z);

				if (visCurr < 0)
				{
					//*table = addToTable(*table, new Cut(I, pointCurr));
					cutApply(hdc, table, I, pointCurr);
				}
				else
				{
					//*table = addToTable(*table, new Cut(I, pointPrev));
					cutApply(hdc, table, I, pointPrev);
				}

				I = IntersectionSegments(*pointPrev,
					*pointCurr, high[j - 1], high[j], z);

				if (visCurr > 0)
				{
					//*table = addToTable(*table, new Cut(I, pointCurr));
					cutApply(hdc, table, I, pointCurr);
				}
				else
				{
					//*table = addToTable(*table, new Cut(I, pointPrev));
					cutApply(hdc, table, I, pointPrev);
				}
			}
			else
			{
				if (visCurr + visPrev > 0)
					I = IntersectionSegments(*pointPrev, *pointCurr,
						high[j - 1], high[j], z);
				else
					I = IntersectionSegments(*pointPrev, *pointCurr,
						low[j - 1], low[j], z);
				if (visCurr)
				{
					//*table = addToTable(*table, new Cut(I, pointCurr));
					cutApply(hdc, table, I, pointCurr);
				}
				else
				{
					//*table = addToTable(*table, new Cut(I, pointPrev));
					cutApply(hdc, table, I, pointPrev);
				}
			}
			pointPrev = pointCurr;
			visPrev = visCurr;
		}
		// Корректировка горизонтов
		//корректировка массивов горизонтов
		debug("mMin", mMin);
		for (int j = 0; j < pointCount; j++)
		{
			X = low[j].getX();
			mMin = min(low[j].getY(), func.f(X, z));

			X = high[j].getX();
			mMax = max(high[j].getY(), func.f(X, z));
			low[j].setY(mMin);
			debug("low[j].getY()", low[j].getY());
			high[j].setY(mMax);
		}
		debug("mMin", mMin);
	}
}


void GorisontAlgo(tFunction &func, Table** table)
{
    int maxHor = -INF;
    int minHor = INF;
    vector<Point> highHorisont;
    vector<Point> lowHorisont;
    for(double i = func.xmin; i <= func.xmax; i += func.dx) {
        highHorisont.push_back(Point(i, maxHor, 0));
        lowHorisont.push_back (Point(i, minHor, 0));
    }
    Point *P = NULL, *Q = NULL;
    bool flag = false;
    double zstep = (func.zmax - func.zmin) / (func.n - 1);
    zstep = Round(zstep);
    int pointCount = lowHorisont.size();
	debug("lowHorisont.size()", lowHorisont.size());

	for(double i = func.zmin; i <= func.zmax + EPS; i += zstep) 
	{
        if(i > func.zmax)
            i = func.zmax;

        //боковые линии
        Point* tmp = new Point(lowHorisont[0].getX(),
			func.f(lowHorisont[0].getX(), i), i);
        if(flag) 
			*table = addToTable(*table, new Cut(tmp, P));

        P = tmp;
        tmp = new Point(lowHorisont[pointCount-1].getX(),
			func.f(lowHorisont[pointCount-1].getX(), i), i);

        if(flag)
			*table = addToTable(*table, new Cut(tmp, Q));

        Q = tmp;
        flag = true;

        Point* first = new Point(lowHorisont[0].getX(),
			func.f(lowHorisont[0].getX(), i), i);

		Point *second, *I;
		// flag1 - 
        int flag1, flag2;
        flag1 = VisiblePoint(lowHorisont[0], highHorisont[0], *first);
        for(int j = 1; j < pointCount; j++) 
		{
            second = new Point(lowHorisont[j].getX(),
				func.f(lowHorisont[j].getX(), i), i);
               // Res.push_back(pair<tPoint, tPoint>(first, second));
            flag2 = VisiblePoint(lowHorisont[j], highHorisont[j], *second);
            if(flag1 == flag2) 
			{
				if (flag2)
				{
					*table = addToTable(*table,
						new Cut(first, second));
				}
            }
			else if (flag2 && flag1)
			{
				I = IntersectionSegments(*first,
					*second, lowHorisont[j - 1],
					lowHorisont[j], i);
				I->setZ(i);

				if (flag2 < 0)
				{
					*table = addToTable(*table,
						new Cut(I, second));
				}
				else
				{
					*table = addToTable(*table,
						new Cut(I, first));
				}

                I = IntersectionSegments(*first,
					*second, highHorisont[j-1],
					highHorisont[j], i);
				I->setZ(i);

				if (flag2 > 0)
				{
					*table = addToTable(*table,
						new Cut(I, second));
				}
				else
				{
					*table = addToTable(*table,
						new Cut(I, first));
				}
            }
            else 
			{
                if(flag2 + flag1 > 0)
                    I = IntersectionSegments(*first, *second,
						highHorisont[j-1], highHorisont[j], i);
                else
                    I = IntersectionSegments(*first, *second,
						lowHorisont[j-1], lowHorisont[j], i);
				I->setZ(i);
				if (flag2)
				{
					*table = addToTable(*table, new Cut(I, second));
				}
				else
				{
					*table = addToTable(*table, new Cut(I, first));
				}
            }
            first = second;
            flag1 = flag2;
        }

        //корректировка массивов горизонтов
		double minM;
		double maxM;
        for(int j = 0; j < pointCount; j++) 
		{
			minM = min(lowHorisont[j].getY(), func.f(lowHorisont[j].getX(), i));
			maxM = max(highHorisont[j].getY(), func.f(highHorisont[j].getX(), i));
			lowHorisont[j].setY(minM);
			highHorisont[j].setY(maxM);
        }
    }
}


int SimpleAlgo(HWND hWnd, tFunction func, Table** table)
{
	HDC hdc = GetDC(hWnd);
	cleanRectOld(hWnd, 0, 0, 300, 0);
    //построение с помощью алгоритма плавающего горизонта
    GorisontAlgo_new(hdc, func, table);

    //корректировка координат с учетом размеров сцены
    double ymax, ymin;
    FindYBroders(*table, ymax, ymin);

	debugDouble("ymax", ymax);
	debugDouble("ymin", ymin);

    Point center((func.xmax - func.xmin)/2 + func.xmin, (ymax - ymin)/2 + ymin,0);
    MovingGraphic(*table, 500, 200);

	RECT rect;
	GetClientRect(hWnd, &rect);

	Scale scale{ 1, 1, 1 };

	/*
	
    double ky = (ymax - ymin)           / (rect.bottom);
    double kx = (func.xmax - func.xmin) / (rect.right);
    double k = 1 / max(kx, ky) * 100;
    Scale scale {1/kx, 1/ky, k};
    ScaleModel(*table, scale);
	*/
	debug("i am here", 0);
	//debugTable(*table, "ddddddd", 0);
	//drawTable(hdc, *table, RGB(0, 255, 0));
	ReleaseDC(hWnd, hdc);

    return 0;
}

