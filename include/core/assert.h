#pragma once

#include <stdbool.h>

enum {
	DFLT_ERROR_CODE = -1,
	DFLT_SUCCESS_CODE = 0,
};

#define assert(cond) do { if (cond) _assert(__FUNCTION__, __LINE__); } while(false)
void _assert(const char * const file, const unsigned line);
