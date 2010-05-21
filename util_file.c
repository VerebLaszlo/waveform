/*
 * @file util_file.c
 *		Contains own function definitions for file operations.
 * @author László Veréb
 * @date 2010.04.19.
 */

#include "util_file.h"

extern const char * program_invocation_short_name;
extern const char * program_invocation_name;

FILE * sfopen(const char *name, const char *mode) {
	FILE *stream;
	errno = 0;
	stream = fopen(name, mode);
	if (stream == NULL) {
		fprintf(stderr, "%s: Couldn't open file %s; %s\n",
				program_invocation_short_name, name, strerror(errno));
		exit(EXIT_FAILURE);
	} else
		return stream;
}

FILE * sfopen_read(const char *name) {
	FILE *stream;
	errno = 0;
	stream = fopen(name, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s: Couldn't open file for reading %s; %s\n",
				program_invocation_short_name, name, strerror(errno));
		exit(EXIT_FAILURE);
	} else
		return stream;
}

FILE * sfopen_write(const char *name) {
	FILE *stream;
	errno = 0;
	stream = fopen(name, "w");
	if (stream == NULL) {
		fprintf(stderr, "%s: Couldn't open file for writing %s; %s\n",
				program_invocation_short_name, name, strerror(errno));
		exit(EXIT_FAILURE);
	} else
		return stream;
}

FILE * sfopen_append(const char *name) {
	FILE *stream;
	errno = 0;
	stream = fopen(name, "a");
	if (stream == NULL) {
		fprintf(stderr, "%s: Couldn't open file for appending %s; %s\n",
				program_invocation_short_name, name, strerror(errno));
		exit(EXIT_FAILURE);
	} else
		return stream;
}
