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

void Cut::setBegin(const int &x, const int &y)noexcept
{
	this->begin = newPoint(x, y);
}

void Cut::setBegin()noexcept
{
	this->begin = newPoint(NO_POINT, NO_POINT);
}

Point* Cut::getBegin()noexcept
{
	return this->begin;
}

int Cut::getBX() const noexcept
{
	return this->begin->x;
}

int Cut::getBY() const noexcept
{
	return this->begin->y;
}

void Cut::setEnd(Point *point)noexcept
{
	this->end = point;
}

void Cut::setEnd(const Point *point)noexcept
{
	this->end = const_cast<Point*>(point);
}

void Cut::setEnd(const int &x,
	const int &y)noexcept
{
	this->end = newPoint(x, y);
}

void Cut::setEnd()noexcept
{
	this->end = newPoint(NO_POINT, NO_POINT);
}

Point* Cut::getEnd()noexcept
{
	return this->end;
}

int Cut::getEX() const noexcept
{
	return this->end->x;
}

int Cut::getEY() const noexcept
{
	return this->end->y;
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

Cut::Cut(int x1, int y1, int x2, int y2)
{
	setBegin(newPoint(x1, y1));
	setEnd(newPoint(x2, y2));
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
int Cut::vectorMultiplication(const Cut &cut) const noexcept
{
	return width() * cut.height() - cut.width() * height();
}

int Cut::vectorMultiplication(Cut *cut) const noexcept
{
	return width() * cut->height() - cut->width() * height();
}

int Cut::scalarMultyplication(const Cut &cut) const noexcept
{
	return width() * cut.width() + height() * cut.height();
}

int Cut::scalarMultyplication(Cut *cut) const noexcept
{
	return width() * cut->width() + height() * cut->height();
}


// конструктор копирования
Cut::Cut(const Cut &cut)
{
	const_cast<Cut&>(cut).status();
	begin = newPoint(cut.begin->x, cut.begin->y);
	end = newPoint(cut.end->x, cut.end->y);
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

int Cut::width() const noexcept
{
	return end->x - begin->x;
}

int Cut::height() const noexcept
{
	return end->y - begin->y;
}

void Cut::debugCut(
	const char* text, int number) noexcept
{
	debug(text, number);
	debug("x1_cut", begin->x);
	debug("y1_cut", begin->y);
	debug("x2_cut", end->x);
	debug("y2_cut", end->y);
}

bool Cut::compareWithCut(
	const Cut &B) noexcept
{
	bool begin = comparePoints(this->begin, B.begin);
	bool end = comparePoints(this->end, B.end);
	return (begin && end);
}

