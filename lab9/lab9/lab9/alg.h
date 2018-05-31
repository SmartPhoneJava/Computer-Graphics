#pragma once
#include "Point.h"
#include "Cut.h"
#include "Table.h"
#include "Debug.h"
#include "Graphics.h"
#include <iostream>

#define SECATEL_NO -2
#define SECATEL_IS_ONE_CUT -3
#define NO_CUTS -4
#define NO_CUTS_AND_SECATEL -5
#define SECATEL_ISNT_CONVEX -6
#define SECATEL_INSIDE_RIGHT 1
#define SECATEL_INSIDE_LEFT -1

int IsConvexFigure(Table *figure);

//Cut *build(Cut* cut, Table *secatel, int r);

Table *build9(HDC hdc, Table* cuts, Table *secatel, int r);