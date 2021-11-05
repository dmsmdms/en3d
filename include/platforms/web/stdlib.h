#pragma once

#define NULL (void *)(0)

_Noreturn void exit(const int code);
void * malloc(unsigned long size);
void free(void * const restrict ptr);
