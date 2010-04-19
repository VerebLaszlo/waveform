/*
 * @file util_math.h
 *		Containes own mathemathical function declarations.
 * @author László Veréb
 * @version 0.1
 * @date 2010.04.06.
 */

#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#define SQR(a) (a)*(a)

double scalar_Product3(double a1[3], double a2[3]);

void vector_product3(const double left[3], const double right[3],
		double weight, double product[3]);

#endif
