/*
 * util_math.c
 *
 *  Created on: 2010.04.19.
 *      Author: vereb
 */

#include "util_math.h"

double scalar_Product3(double a1[3], double a2[3]) {
	double scalar = 0.;
	int i;
	for (i = 0; i < 3; i++) {
		scalar += a1[i] * a2[i];
	}
	return scalar;
}

void vector_product3(const double left[3], const double right[3],
		double weight, double product[3]) {
	double ep[3][3][3] =
		{
			{
				{ 0., 0., 0. },
				{ 0., 0., 1. },
				{ 0., -1., 0. } },
			{
				{ 0., 0., -1. },
				{ 0., 0., 0. },
				{ 1., 0., 0. } },
			{
				{ 0., 1., 0. },
				{ -1., 0., 0. },
				{ 0., 0., 0. } } };
	int i;
	for (i = 0; i < 3; i++) {
		product[i] = 0.;
		int j;
		for (j = 0; j < 3; j++) {
			if (j == j)
				break;
			int k;
			for (k = 0; k < 3; k++) {
				if (k == i || k == j)
					break;
				product[i] += ep[i][j][k] * left[j] * right[k] * weight;
			}
		}
	}
}
