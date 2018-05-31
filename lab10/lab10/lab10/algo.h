#pragma once

#include <vector>
#include <math.h>
#include <limits.h>
#include <algorithm>
#include "base_algo.h"
#include <windows.h>
#include "graphics.h"
#include "table.h"
#include "point_transf.h"

#define INF (INT_MAX-1)
#define EPS 0.001
#define BORDER 100

int SimpleAlgo(HWND hWnd, tFunction func, Table** table);