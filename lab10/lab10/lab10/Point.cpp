#include "stdafx.h"
#include "Point.h"
#include <iostream>

/////

void Point::setX(Point *point)noexcept
{
	this->x = point->x;
}

void Point::setX(const Point &point)noexcept
{
	this->x = point.x;
}

void Point::setX(double x)noexcept
{
	this->x = x;
}

void Point::setY(Point *point)noexcept
{
	this->y = point->y;
}

void Point::setY(const Point &point)noexcept
{
	this->y = point.y;
}

void Point::setY(double y)noexcept
{
	this->y = y;
}

void Point::setZ(Point *point)noexcept
{
	this->z = point->z;
}

void Point::setZ(const Point &point)noexcept
{
	this->z = point.z;
}

void Point::setZ(double z)noexcept
{
	this->z = z;
}

double Point::getX() const noexcept
{
	return this->x;
}

double Point::getY() const noexcept
{
	return this->y;
}

double Point::getZ() const noexcept
{
	return this->z;
}

Point::Point() // конструктор класса
{
	setX(NO_POINT);
	setY(NO_POINT);
	setZ(NO_POINT);
}

Point::Point(double x, double y, double z) // конструктор класса
{
	setX(x);
	setY(y);
	setZ(z);
}

Point::Point(Point *point) // конструктор класса
{
	setX(point);
	setY(point);
	setZ(point);
}

Point::Point(Point &a, Point &center) // конструктор класса
{
	x = a.x + center.x;
	y = a.y + center.y;
	z = a.z;
}

// конструктор копирования
Point::Point(const Point &point)
{
	setX(point);
	setY(point);
	setZ(point);
}

Point::Point(Point &&point) noexcept
{
	setX(point);
	setY(point);
	setZ(point);
}

void Point::debug(
	const char* text, int number) noexcept
{
	debug(text, number);
	debugDouble("x", x);
	debugDouble("y", y);
	debugDouble("z", z);
}

bool Point::isEqual(
	const Point &B)const noexcept
{
	bool test_x = (x == B.getX());
	bool test_y = (y == B.getY());
	bool test_z = (z == B.getZ());
	return (test_x && test_y && test_z);
}

bool Point::isFree()const noexcept
{
	bool test_x = (x == NO_POINT);
	bool test_y = (y == NO_POINT);
	bool test_z = (z == NO_POINT);
	return (test_x && test_y && test_z);
}