/*
 * util_file.h
 *
 *  Created on: 2010.04.19.
 *      Author: vereb
 */

#ifndef UTIL_FILE_H_
#define UTIL_FILE_H_

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
FILE * sfopen_read(char *name);

/**
 *		The function is a secure interface for the system function fopenwith
 *	write mode.
 * @param[in] name	: filename to open
 * @return	pointer to the opened file
 */
FILE * sfopen_write(char *name);

/**
 *		The function is a secure interface for the system function fopenwith
 *	append mode.
 * @param[in] name	: filename to open
 * @return	pointer to the opened file
 */
FILE * sfopen_append(char *name);

#endif /* UTIL_FILE_H_ */
