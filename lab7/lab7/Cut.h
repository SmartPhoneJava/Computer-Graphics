#pragma once

#include "Point.h"
#include "Visibility.h"
#include "Debug.h"

#define VERTICAL 1000000
#define HORIZONTAL 0

class Cut
{
private:
	Point *begin;
	Point *end;

	int width() const noexcept;

	int height() const noexcept;

	void destroy() noexcept;

public:
	/*������������ */
	Cut();
	Cut(Point *p1, Point *p2);
	Cut(int x1, int y1, int x2, int y2);
	Cut(const Point *p1, const Point *p2);
	Cut(const Cut &cut);
	~Cut();

	/*��������� ���� Begin*/
	void setBegin(Point *point) noexcept;
	void setBegin(const Point *point) noexcept;
	void setBegin(const int &x, const int &y) noexcept;
	void setBegin() noexcept;

	/*��������� �������� ���� Begin*/
	Point* getBegin() noexcept;

	/*��������� ���� End*/
	void setEnd(Point *point) noexcept;
	void setEnd(const Point *point) noexcept;
	void setEnd(const int &x, const int &y) noexcept;
	void setEnd() noexcept;

	void status() noexcept;

	/*��������� ���� End*/
	Point* getEnd() noexcept;
	
	Cut(Cut &&cut) noexcept;

	Cut& operator=(const Cut& other) noexcept;

	Cut& operator=(Cut&& other)noexcept;

	void debugCut(
		const char* text, int number)noexcept;

	void debugCutVisibility(
		const char* text, int number)noexcept;

	bool compareWithCut(
		const Cut &B)noexcept;

	double tan() const noexcept;

};

