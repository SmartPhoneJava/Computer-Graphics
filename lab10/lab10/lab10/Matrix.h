#pragma once
#include "Resource.h"

//��� ��� �������� ������ �������������� � N-������ ��-��
typedef double t_matrix[DIMENSION][DIMENSION];

void multMatrix(t_matrix res,
	const t_matrix a, const t_matrix b);

void copyMatrix(t_matrix res, const t_matrix a);