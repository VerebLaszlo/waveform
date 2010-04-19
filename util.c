#include "util.h"

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


