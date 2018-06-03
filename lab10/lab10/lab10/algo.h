#pragma once

#include <vector>
#include <math.h>
#include <limits.h>
#include <algorithm>
#include <windows.h>
#include "graphics.h"
#include "table.h"
#include "f.h"
#include "Transformation.h"

#define INF (INT_MAX-1)
#define EPS 0.001
#define BORDER 100

int SimpleAlgo(HWND hWnd, tFunction func, Table** table);