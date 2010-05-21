/*
 * @file util_file.h
 *		Containes own fonctions for file operations.
 * @author László Veréb
 * @date 2010.04.19.
 */

#ifndef UTIL_FILE_H_
#define UTIL_FILE_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/**
 *		The function is a secure interface for the system function fopen.
 * @param[in] name	: filename to open
 * @param[in] mode	: in which mode to open (r,w,a)
 * @return	pointer to the opened file
 */
FILE * sfopen(const char* const name, const char* const mode);

/**
 *		The function is a secure interface for the system function fopenwith
 *	read mode.
 * @param[in] name	: filename to open
 * @return	pointer to the opened file
 */
FILE * sfopen_read(const char *name);

/**
 *		The function is a secure interface for the system function fopenwith
 *	write mode.
 * @param[in] name	: filename to open
 * @return	pointer to the opened file
 */
FILE * sfopen_write(const char *name);

/**
 *		The function is a secure interface for the system function fopenwith
 *	append mode.
 * @param[in] name	: filename to open
 * @return	pointer to the opened file
 */
FILE * sfopen_append(const char *name);

#endif /* UTIL_FILE_H_ */
