/*
 * @file util.h
 *		Containes own functions declarations.
 * @author László Veréb
 * @date 2010.04.06.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>

/**
 *		Enumeration for boolean type.
 */
typedef enum {
	null = -1, false = 0, true = 1
} bool;

/**
 *		The function calculates the conjuction of the boolean vector's elements.
 * @param[in] array		: boolean vector
 * @param[in] length	: length of the vector
 * @return	conjuction of the elements
 */
bool multi_And(bool array[], size_t length);

/**
 *		The function calculates the disjuction of the boolean vector's elements.
 * @param[in] array		: boolean vector
 * @param[in] length	: length of the vector
 * @return	disjuction of the elements
 */
bool multi_Or(bool array[], size_t length);

/**
 *		The function generates a random number from the [0,1) interval.
 * @return random number
 */
double rand1(void);

#endif