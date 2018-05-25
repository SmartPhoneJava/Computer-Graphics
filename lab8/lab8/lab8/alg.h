#pragma once
#include "Point.h"
#include "Cut.h"
#include "Debug.h"
#include "Secatel.h"
#include <iostream>

Cut* cutInside(Cut *cut, Secatel sec);

bool nearAB(int a, int b);

void setIfBool(int &a, int set, bool b);