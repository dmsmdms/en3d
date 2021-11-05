#pragma once

#include <stdarg.h>

typedef struct file FILE;
extern FILE * restrict stdout;
extern FILE * restrict stderr;

int vfprintf(FILE * const file, const char * restrict format, va_list args);
int fprintf(FILE * const file, const char * const format, ...);
int printf(const char * const format, ...);
int putc(const char c, FILE * const restrict file);
int puts(const char * const str);
int putchar(const char c);

unsigned long fwrite(const void * data, const unsigned long size, unsigned long count, FILE * const file);
FILE * fdopen(const int fd, const char * const str);
int fclose(FILE * const restrict file);
int fflush(FILE * const restrict file);
