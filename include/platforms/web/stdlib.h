#pragma once

#include <stddef.h>

_Noreturn void exit(const int code);
void * malloc(unsigned long size);
void free(void * const restrict ptr);
