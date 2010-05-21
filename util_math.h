/*
 * @file util_math.h
 *		Containes own mathemathical functions declarations.
 * @author László Veréb
 * @date 2010.04.06.
 */

#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <math.h>
#include <stdlib.h>

/**
 *		The macrofunction returns the square of the argument.
 * @param[in] a	 : the number
 * @return the square of the number
 */
#define SQR(a) ((a)*(a))

/**
 *		The function returns the square of the argument.
 * @param[in] a	 : the number
 * @return the square of the number
 */
double sqr(double a);

/**
 *		The function rounds the number to the nearest power of two.
 * @param[in] num	: number to be rounded
 * @return the rounded value
 */
long round_pow2(double num);

/**
 *		The function returns the lowest element in the vector.
 * @param[in] array	 : the vector
 * @param[in] length : the length of the vector
 * @return the lowest element
 */
size_t multi_Min(size_t array[], size_t length);

/**
 *		The function returns the highest element in the vector.
 * @param[in] array	 : the vector
 * @param[in] length : the length of the vector
 * @return the highest element
 */
size_t multi_Max(size_t array[], size_t length);

/**
 *		The function calculates the scalar product of two vectors.
 * @param[in]  left		: the left vector
 * @param[in]  right	: the right vector
 * @return	the product
 */

double scalar_Product3(const double a1[], const double a2[3]);

/**
 *		The function calculates the wightened vector product of two vectors.
 * @param[in]  left		: the left vector
 * @param[in]  right	: the right vector
 * @param[in]  weight	: the product is multiplied by this value
 * @param[out] product	: the wightened product
 */
void vector_product3(const double left[3], const double right[3],
		double weight, double product[3]);

#endif
