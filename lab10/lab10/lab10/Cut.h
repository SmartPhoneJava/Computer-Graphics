#pragma once

#include "Point.h"
#include "Debug.h"

class Cut
{
private:
	Point *begin;
	Point *end;

	void destroy() noexcept;

public:
	/*Конструкторы */
	Cut();
	Cut(Point *p1, Point *p2);
	Cut(double x1, double y1, double z1,
		double x2, double y2, double z2);
	Cut(const Point *p1, const Point *p2);
	Cut(const Cut &cut);
	~Cut();

	/*Установка поля Begin*/
	void setBegin(Point *point) noexcept;
	void setBegin(const Point *point) noexcept;
	void setBegin(const double &x, const double &y,
		const double &z) noexcept;
	void setBegin() noexcept;

	/*Получение значения поля Begin*/
	Point* getBegin() noexcept;

	double getBX() const noexcept;
	double getBY() const noexcept;
	double getBZ() const noexcept;

	/*Установка поля End*/
	void setEnd(Point *point) noexcept;
	void setEnd(const Point *point) noexcept;
	void setEnd(const double &x, const double &y,
		const double &z) noexcept;
	void setEnd() noexcept;

	void setBX(double x) noexcept;
	void setBY(double y) noexcept;
	void setBZ(double z) noexcept;

	void setEX(double x) noexcept;
	void setEY(double y) noexcept;
	void setEZ(double z) noexcept;

	void status() noexcept;

	/*Получение поля End*/
	Point* getEnd() noexcept;

	double getEX() const noexcept;
	double getEY() const noexcept;
	double getEZ() const noexcept;
	
	Cut(Cut &&cut) noexcept;

	Cut& operator=(const Cut& other) noexcept;

	Cut& operator=(Cut&& other)noexcept;

	void debug(
		const char* text, int number)noexcept;

	double dx() const noexcept;

	double dy() const noexcept;

	double dz() const noexcept;

	// Векторное произведение
	Point* vectorMultiplication(const Cut &cut) const noexcept;

	// Скалярное произведение
	int scalarMultyplication(const Cut &cut) const noexcept;

	bool isEqual(const Cut &B)noexcept;
};

