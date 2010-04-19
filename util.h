#ifndef UTIL_H
#define UTIL_H

typedef enum {
	null = -1, false = 0, true = 1
} bool;

/**
 *		The function generates a random number from the [0,1) interval.
 * @return random number
 */
double rand1(void);

/**
 *		The function rounds the number to the nearest power of two.
 * @param[in] num	: number to be rounded
 * @return the rounded value
 */
long round_po2(double num);
#endif
