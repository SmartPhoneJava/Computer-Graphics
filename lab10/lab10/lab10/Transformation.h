#pragma once
#include "Table.h"
#include "Point.h"
#include "Matrix.h"
#include "Vector.h"
#include <math.h>

using namespace std;

struct Rotate {
    double x_angle;
    double y_angle;
    double z_angle;
};
struct Scale {
    double kx;
    double ky;
    double kz;
};

void RotateModel(Table *table, const Rotate &act);
void ScaleModel (Table *table, const Scale &act);
void scalingMatrix(t_matrix a, const Scale &act);

