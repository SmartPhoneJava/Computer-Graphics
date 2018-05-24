#include "stdafx.h"
#include "Visibility.h"

// Видима ли точка. 0 если видима, >0 если невидима
int isPointVisible(Visibility vis)
{
	int result = ((vis.T1 + vis.T2 + vis.T3 + vis.T4) == 0);
	return result;
}

// Является ли отрезок частично видимым
bool isCutPartVisible(Visibility v1, Visibility v2)
{
	int count = v1.T1 * v2.T1 + v1.T2 * v2.T2 + v1.T3 * v2.T3 + v1.T4 * v2.T4;
	bool result = (count == 0);
	return result;
}

// Степень видимости отрезка(полная видимость\невидимость, частичная)
int checkCut(Visibility vis1, Visibility vis2)
{
	int first = isPointVisible(vis1);
	int second = isPointVisible(vis2);

	int result = PART_VISIBLE;

	if ((first + second) == FULL_VISIBLE)
		result = FULL_VISIBLE;
	else if ((first != FULL_VISIBLE) && (second != FULL_VISIBLE))
	{
		if (!isCutPartVisible(vis1, vis2))
			result = FULL_UNVISIBLE;
	}

	return result;
}

void debugVisibility(Visibility visibility,
	const char* text, int number)
{
	debug(text, number);
	debug("is_left", visibility.T1);
	debug("is_right", visibility.T2);
	debug("is_up", visibility.T3);
	debug("is_down", visibility.T4);
}

Visibility* newVisibility(int t1,
	int t2, int t3, int t4)
{
	Visibility *visibility = (Visibility*)calloc(1, sizeof(Visibility));
	visibility->T1 = t1;
	visibility->T2 = t2;
	visibility->T3 = t3;
	visibility->T4 = t4;
	return visibility;
}

void deleteVisibility(Visibility **v)
{
	free(*v);
	*v = NULL;
}