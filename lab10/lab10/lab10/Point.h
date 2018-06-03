#pragma once
#include "Debug.h"
#include "Resource.h"
#include "Matrix.h"
#include "Vector.h"

class Point
{
private:
	double x, y, z;

public:
	/*Конструкторы */
	Point();
	Point(Point *p);
	Point(const Point &point);
	Point(double x, double y, double z);
	Point(Point &a, Point &center);

	void setX(Point *point) noexcept;
	void setX(const Point *point) noexcept;
	void setX(const Point &point) noexcept;
	void setX(double x) noexcept;

	void setY(Point *point) noexcept;
	void setY(const Point *point) noexcept;
	void setY(const Point &point) noexcept;
	void setY(double y) noexcept;

	void setZ(Point *point) noexcept;
	void setZ(const Point *point) noexcept;
	void setZ(const Point &point) noexcept;
	void setZ(double z) noexcept;

	double getX() const noexcept;
	double getY() const noexcept;
	double getZ() const noexcept;

	Point(Point &&point) noexcept;

	void debug(
		const char* text, int number)noexcept;

	bool isEqual(
		const Point &B)const noexcept;

	bool isFree()const noexcept;

	Point& operator=(const Point& other) noexcept;

	Point& operator=(Point&& other)noexcept;

	void update(const t_matrix &matrix);
};