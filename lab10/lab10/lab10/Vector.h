#pragma once
#include "Resource.h"
#include "Matrix.h"

//��� ��� �������� �������-����� � N-������ ��-��
typedef double t_vect[DIMENSION];

void multVector(t_vect vec, const t_matrix a);
