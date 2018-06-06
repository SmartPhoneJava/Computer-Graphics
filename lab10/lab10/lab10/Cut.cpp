#include "stdafx.h"
#include "Cut.h"
#include <iostream>

void Cut::setBegin(Point *point)noexcept
{
	this->begin = point;
}

void Cut::setBegin(const Point *point)noexcept
{
	this->begin = const_cast<Point*>(point);
}

void Cut::setBegin(const double &x, const double &y,
	const double &z)noexcept
{
	this->begin = new Point(x, y, z);
}

void Cut::setBegin()noexcept
{
	this->begin = new Point(NO_POINT, NO_POINT, NO_POINT);
}

Point* Cut::getBegin()noexcept
{
	return this->begin;
}

double Cut::getBX() const noexcept
{
	return this->begin->getX();
}

double Cut::getBY() const noexcept
{
	return this->begin->getY();
}

double Cut::getBZ() const noexcept
{
	return this->begin->getZ();
}

void Cut::setEnd(Point *point)noexcept
{
	this->end = point;
}

void Cut::setEnd(const Point *point)noexcept
{
	this->end = const_cast<Point*>(point);
}

void Cut::setEnd(const double &x,
	const double &y, const double &z)noexcept
{
	this->end = new Point(x, y, z);
}

void Cut::setEnd()noexcept
{
	this->end = new Point(NO_POINT, NO_POINT, NO_POINT);
}

Point* Cut::getEnd()noexcept
{
	return this->end;
}

double Cut::getEX() const noexcept
{
	return this->end->getX();
}

double Cut::getEY() const noexcept
{
	return this->end->getY();
}

double Cut::getEZ() const noexcept
{
	return this->end->getZ();
}

void Cut::setBX(double x) noexcept
{
	begin->setX(x);
}
void Cut::setBY(double y) noexcept
{
	begin->setY(y);
}
void Cut::setBZ(double z) noexcept
{
	begin->setZ(z);
}

Cut::Cut(Point &p1, Point &p2)
{
	begin = &p1;
	end = &p2;
}

void Cut::setEX(double x) noexcept
{
	end->setX(x);
}
void Cut::setEY(double y) noexcept
{
	end->setY(y);
}
void Cut::setEZ(double z) noexcept
{
	end->setZ(z);
}

Cut::Cut() // конструктор класса
{
	setBegin();
	setEnd();
}

Cut::Cut(const Point *p1, const Point *p2)
{
	setBegin(p1);
	setEnd(p2);
}

Cut::Cut(double x1, double y1, double z1,
	double x2, double y2, double z2)
{
	setBegin(new Point(x1, y1, z1));
	setEnd(new Point(x2, y2, z2));
}

Cut::Cut(Point *p1, Point *p2)
{
	setBegin(p1);
	setEnd(p2);
}

void Cut::status() noexcept
{
	if (!begin)
	{
		debug("begin problem", 0);
	}
	if (!end)
	{
		debug("end problem", 0);
	}
}

// (a.y * b.z - a.z* b.y) - (a.x * b.z - a.z * b.x)
// + (a.x * b.y - a.y * b.x)
Point* Cut::vectorMultiplication(const Cut &cut) const noexcept
{
	double x = dy() * cut.dz() - dz() * cut.dy();
	double y = dz() * cut.dx() - dx() * cut.dz();
	double z = dx() * cut.dy() - dy() * cut.dx();
	return new Point(x, y, z);
}

int Cut::scalarMultyplication(const Cut &cut) const noexcept
{
	return dx() * cut.dx() + dy() * cut.dy() + dz() * cut.dz();
}


// конструктор копирования
Cut::Cut(const Cut &cut)
{
	const_cast<Cut&>(cut).status();
	begin = new Point(cut.begin);
	end = new Point(cut.end);
}

void Cut::destroy() noexcept
{
	if (begin)
		delete begin;
	if (end)
		delete end;
}

Cut::Cut(Cut &&cut) noexcept
{
	begin = cut.begin;
	end = cut.end;
	cut.begin = nullptr;
	cut.end = nullptr;
}

Cut& Cut::operator=(const Cut& other) noexcept
{
	destroy();
	begin = other.begin;
	end = other.end;
	return *this;
}

Cut& Cut::operator=(Cut&& other)noexcept
{
	destroy();
	begin = other.begin;
	end = other.end;
	other.begin = nullptr;
	other.end = nullptr;
	return *this;
}

Cut::~Cut() // деструктор класса
{
	destroy();
}

double Cut::dx() const noexcept
{
	return end->getX() - begin->getX();
}

double Cut::dy() const noexcept
{
	return end->getY() - begin->getY();
}

double Cut::dz() const noexcept
{
	return end->getZ() - begin->getZ();
}

// Обновление позиции с помощью матрицы
void Cut::update(const t_matrix &matrix)
{
	begin->update(matrix);
	end->update(matrix);
}

void Cut::debug(
	const char* text, int number) noexcept
{
	//debug(text, number);
	begin->debug("Cut Point ", 1);
	end->debug("Cut Point ", 2);
}

bool Cut::isEqual(
	const Cut &B) noexcept
{
	bool begin = this->begin->isEqual(B.begin);
	bool end = this->end->isEqual(B.end);
	return (begin && end);
}

