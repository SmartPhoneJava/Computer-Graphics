#pragma once
#include "Resource.h"

//тип для хранения матриц преобразование в N-мерном пр-ве
typedef double t_matrix[DIMENSION][DIMENSION];

void multMatrix(t_matrix res,
	const t_matrix a, const t_matrix b);

void copyMatrix(t_matrix res, const t_matrix a);