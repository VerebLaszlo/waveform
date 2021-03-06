/**
 * @file util_debug.c
 *		Containes simple macros for debugging.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

/**		Writes the current file name and line number.
 */
#define ERR_LINE					\
fprintf(stderr, "%s %d\n", 			\
		__FILE__, __LINE__);		\
fflush(stderr);

/**		Writes the string after the current file name and line number without
 * the newline character.
 */
#define ERR_STR(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %s ",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**		Writes the string after the current file name and line number.
 */
#define ERR_STR_END(X)				\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %s\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**		Writes the floating point number after the current file name and line number.
 */
#define ERR_GL(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %lg\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**		Writes the digital number after the current file name and line number.
 */
#define ERR_D(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %d\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

/**		Writes the pointer address after the current file name and line number.
 */
#define ERR_P(X)					\
if (DEBUG==1)						\
	fprintf(stderr, "%s %d: %p\n",	\
			__FILE__, __LINE__, X);	\
	fflush(stderr);

#endif /* debug.h */
