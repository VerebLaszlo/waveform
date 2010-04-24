/*
 * @file util.c
 *		Containes own mathemathical functions definitions.
 * @author László Veréb
 * @date 2010.04.06.
 */
 
#include "util.h"

bool multi_And(bool array[], size_t length) {
	size_t i;
	for (i = 0; i < length; i++) {
		if (!array[i]) {
			return false;
		}
	}
	return true;
}

bool multi_Or(bool array[], size_t length) {
	size_t i;
	for (i = 0; i < length; i++) {
		if (array[i]) {
			return true;
		}
	}
	return false;
}

double rand1(void) {
	return ((double)rand()) / ((double)RAND_MAX);
}

long round_po2(double num) {
	register double temp = log(num) / M_LN2;
#ifdef __USE_ISOC99
	return (long) exp2(ceil(temp));
#endif
	//return (long) pow(2., ceil(temp));
	return (long) exp(ceil(temp) * M_LN2);
}


