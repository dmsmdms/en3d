#pragma once

#include <stdbool.h>

typedef struct {
	bool is_fullscreen;
} options_t;

extern options_t options;
void options_parse(const int argc, char * const * const argv);
