#include <core/assert.h>
#include <stdlib.h>
#include <stdio.h>

void _assert(const char * const file, const unsigned line) {
	fprintf(stderr, "ERROR: %s:%u\n", file, line);
	exit(DFLT_ERROR_CODE);
}
