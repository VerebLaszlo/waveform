#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

/**
 * \todo írni
 */
#define ERR_LINE					\
fprintf(stderr, "%s %d\n", 			\
		__FILE__, __LINE__);		\
fflush(stderr);

/**
 * \todo írni
 */
#define ERR_STR(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %s ",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**
 * \todo írni
 */
#define ERR_STR_END(X)				\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %s\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**
 * \todo írni
 */
#define ERR_GL(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %lg\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**
 * \todo írni
 */
#define ERR_D(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %d\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**
 * \todo írni
 */
#define ERR_P(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %p\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

#endif /* debug.h */
