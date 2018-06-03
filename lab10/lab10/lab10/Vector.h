#pragma once
#include "Resource.h"
#include "Matrix.h"

//тип для хранения вектора-точки в N-мерном пр-ве
typedef double t_vect[DIMENSION];

void multVector(t_vect vec, const t_matrix a);
