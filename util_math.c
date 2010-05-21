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

long round_pow2(double num) {
	register double temp = log(num) / M_LN2;
#ifdef __USE_ISOC99
	return (long) exp2(ceil(temp));
#endif
	//return (long) pow(2., ceil(temp));
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

void vector_product3(const double left[3], const double right[3],
		double weight, double product[3]) {
	product[0] = weight * (left[1] * right[2] - left[2] * right[1]);
	product[1] = weight * (left[2] * right[0] - left[0] * right[2]);
	product[2] = weight * (left[0] * right[1] - left[1] * right[0]);
}
