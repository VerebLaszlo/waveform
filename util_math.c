/*
 * @file util_math.c
 *		Contains own mathematical function definitions.
 * @author László Veréb
 * @date 2010.04.19.
 */

#include "util_math.h"

double sqr(double a) {
	return a * a;
}

size_t multi_Min(size_t array[], size_t length) {
	size_t i, min = array[0];
	for (i = 1; i < length; i++) {
		min = fmin(min, array[i]);
	}
	return min;
}

size_t multi_Max(size_t array[], size_t length) {
	size_t i, max = array[0];
	for (i = 1; i < length; i++) {
		max = fmax(max, array[i]);
	}
	return max;
}

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
