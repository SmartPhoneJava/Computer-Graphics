#include "stdafx.h"
#include "Transformation.h"

/*
//https://habr.com/post/319144/
*/
void rotatingMatrix(t_matrix a, const Rotate &act)
{
    t_matrix ax = { { 1, 0, 0 },
                       { 0, cos(act.x_angle), -sin(act.x_angle) },
                       { 0, sin(act.x_angle), cos(act.x_angle) } };
    t_matrix ay = { { cos(act.y_angle), 0, sin(act.y_angle) },
                       { 0, 1, 0 },
                       { -sin(act.y_angle), 0, cos(act.y_angle) } };
    t_matrix az = { { cos(act.z_angle), -sin(act.z_angle), 0 },
                       { sin(act.z_angle), cos(act.z_angle), 0 },
                       { 0, 0, 1 } };
    t_matrix tmp;
    multMatrix(tmp, ax, ay);
	multMatrix(a, tmp, az);
}

void scalingMatrix(t_matrix a, const Scale &act)
{
	t_matrix m = {
		{ act.kx, 0, 0 },
		{ 0, act.ky, 0 },
		{ 0, 0, act.kz }
	};
	copyMatrix(a, m);
}

void RotateModel(Table *table, const Rotate &act)
{
    t_matrix m;
	rotatingMatrix(m, act);
	Table *move = table;
	while (move)
	{
		move->cut->update(m);
		move = move->next;
	}
}

void ScaleModel(Table *table, const Scale &act)
{
    t_matrix m;
	scalingMatrix(m, act);
	Table *move = table;
	while (move)
	{
		move->cut->update(m);
		move = move->next;
	}
}
