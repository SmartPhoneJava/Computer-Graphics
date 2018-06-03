#include "stdafx.h"
#include "Vector.h"

void multVector(t_vect vec, const t_matrix a)
{
	t_vect res = { 0 };
	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {
			res[i] += a[i][j] * vec[j];
		}
	}
	for (int i = 0; i < DIMENSION; i++) {
		vec[i] = res[i];
	}
}