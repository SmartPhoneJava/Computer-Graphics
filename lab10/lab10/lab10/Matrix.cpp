#include "stdafx.h"
#include "Matrix.h"

void multMatrix(t_matrix res, const t_matrix a, const t_matrix b)
{
	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {
			res[i][j] = 0;
			for (int k = 0; k < DIMENSION; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void copyMatrix(t_matrix res, const t_matrix a)
{
	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {
			res[i][j] = a[i][j];
		}
	}
}