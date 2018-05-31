#include "algo.h"

void FindYBroders(Table *table, double &ymax, double &ymin)
{
    ymax = ymin = table->cut->getBegin()->getY();
	Table* move = table;
	double y_end, y_begin;
	while (move)
	{
		y_begin = move->cut->getBegin()->getY();
		y_end = move->cut->getEnd()->getY();

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

Point* IntersectionSegments(Point &p1, Point &p2, Point &p3, Point &p4)
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
    //res.z = p1.z;
    return new Point(x, y, 0);
}
double Round(double x) //magic
{
    x *= 100000;
    x = round(x);
    x /= 100000;
    return x;
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
    Point *P, *Q;
    bool flag = false;
    double zstep = (func.zmax - func.zmin) / (func.n - 1);
    zstep = Round(zstep);
    int pointCount = lowHorisont.size();
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
                if(flag2)
					*table = addToTable(*table, new Cut(first, second));
            }
            else if(flag2 && flag1) 
			{
                I = IntersectionSegments(*first,
					*second, lowHorisont[j-1], lowHorisont[j]);
                I->setZ(i);

                if(flag2 < 0)
					*table = addToTable(*table, new Cut(I, second));
                else
					*table = addToTable(*table, new Cut(I, first));

                I = IntersectionSegments(*first,
					*second, highHorisont[j-1], highHorisont[j]);
				I->setZ(i);

                if(flag2 > 0)
					*table = addToTable(*table, new Cut(I, second));
                else
					*table = addToTable(*table, new Cut(I, first));
            }
            else 
			{
                if(flag2 + flag1 > 0)
                    I = IntersectionSegments(*first, *second,
						highHorisont[j-1], highHorisont[j]);
                else
                    I = IntersectionSegments(*first, *second,
						lowHorisont[j-1], lowHorisont[j]);
				I->setZ(i);
                if(flag2)
					*table = addToTable(*table, new Cut(I, second));
                else
					*table = addToTable(*table, new Cut(I, first));
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
	cleanRectOld(hWnd, 0, 0, 300, 0);
    //построение с помощью алгоритма плавающего горизонта
    GorisontAlgo(func, table);

    //корректировка координат с учетом размеров сцены
    double ymax, ymin;
    FindYBroders(*table, ymax, ymin);

    Point center((func.xmax - func.xmin)/2 + func.xmin, (ymax - ymin)/2 + ymin,0);
    MovingGraphic(*table, -center.getX(), -center.getY());

	RECT rect;
	GetClientRect(hWnd, &rect);

	
    double ky = (ymax - ymin)           / (rect.bottom - BORDER * 2);
    double kx = (func.xmax - func.xmin) / (rect.right - BORDER * 2);
    double k = 1 / max(kx, ky);
    Scale scale {k, k, k};
    ScaleModel(*table, scale);

	HDC hdc= GetDC(hWnd);
	drawTable(hdc, *table, RGB(0, 255, 0));
	ReleaseDC(hWnd, hdc);

    return 0;
}

