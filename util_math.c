/**
 * @file util_math.c
 *		Contains own mathematical function definitions.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "util_math.h"

double sqr(double a) {
	return a * a;
}

long round_pow2(double num) {
	register double temp = log(num) / M_LN2;
#ifdef __USE_ISOC99
	return (long) exp2(ceil(temp));
#endif
	return (long) exp(ceil(temp) * M_LN2);
}

size_t multi_Min(size_t array[], size_t length) {
	size_t i, min = array[0];
	for (i = 1; i < length; i++) {
		min = min < array[i] ? min : array[i];
	}
	return min;
}

size_t multi_Max(size_t array[], size_t length) {
	size_t i, max = array[0];
	for (i = 1; i < length; i++) {
		max = max > array[i] ? max : array[i];
	}
	return max;
}

double scalar_Product3(const double a1[3], const double a2[3]) {
	return a1[0] * a2[0] + a1[1] * a2[1] + a1[2] * a2[2];
}

void vector_product3(const double a1[3], const double a2[3],
		double weight, double product[3]) {
	product[0] = weight * (a1[1] * a2[2] - a1[2] * a2[1]);
	product[1] = weight * (a1[2] * a2[0] - a1[0] * a2[2]);
	product[2] = weight * (a1[0] * a2[1] - a1[1] * a2[0]);
}
