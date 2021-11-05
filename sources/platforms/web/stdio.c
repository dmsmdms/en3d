#include <core/assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef WEB_FSTREAM_BUF_SIZE
#warning WEB_FSTREAM_BUF_SIZE is undefined
#define WEB_FSTREAM_BUF_SIZE 1024
#endif

FILE * restrict stdout = NULL;
FILE * restrict stderr = NULL;

struct file {
	char * restrict cur;
	char * start;
	char * end;
	int fd;
};

FILE * fdopen(const int fd, const char * const str) {
	FILE * const restrict file = malloc(sizeof(FILE));

	if (file == NULL) {
		return NULL;
	}

	file->start = malloc(WEB_FSTREAM_BUF_SIZE);

	if (file->start == NULL) {
		return NULL;
	}

	file->end = file->start + WEB_FSTREAM_BUF_SIZE;
	file->cur = file->start;
	file->fd = fd;
	(void)str;

	return file;
}

int fclose(FILE * const restrict file) {
	const int result = fflush(file);

	free(file->start);
	free(file);

	return result;
}

int fflush(FILE * const restrict file) {
	const unsigned length = file->cur - file->start;

	if (length > 0) {
		if (write(file->fd, file->start, length) < 0) {
			return DFLT_ERROR_CODE;
		}

		file->cur = file->start;
	}

	return DFLT_SUCCESS_CODE;
}

static int _putu(FILE * const file, unsigned arg) {
	char buf[32], * restrict buf_ptr = buf;

	do {
		*buf_ptr++ = (arg % 10) + '0';
		arg /= 10;
	} while(arg != 0);

	do {
		putc(*(--buf_ptr), file);
	} while (buf_ptr > buf);

	return DFLT_SUCCESS_CODE;
}

static int _puts(FILE * const file, const char * restrict str) {
	while (*str != '\0') {
		if (putc(*str++, file) != DFLT_SUCCESS_CODE) {
			return DFLT_ERROR_CODE;
		}
	}

	return DFLT_SUCCESS_CODE;
}

int putc(const char c, FILE * const restrict file) {
	if (file->cur >= file->end) {
		if (fflush(file) != DFLT_SUCCESS_CODE) {
			return DFLT_ERROR_CODE;
		}
	}

	*file->cur++ = c;
	return DFLT_SUCCESS_CODE;
}

unsigned long fwrite(const void * data, const unsigned long size, unsigned long count, FILE * const file) {
	unsigned long counter;

	for (counter = 0; count > 0; --count) {
		if (file->cur + size >= file->end) {
			if (fflush(file) != DFLT_SUCCESS_CODE) {
				return counter;
			}
		}

		memcpy(file->cur, data, size);
		file->cur += size;
		counter += size;
		data += size;
	}

	if (file == stderr) {
		if (fflush(stderr) != DFLT_SUCCESS_CODE) {
			return DFLT_ERROR_CODE;
		}
	}

	return counter;
}

int putchar(const char c) {
	return putc(c, stdout);
}

int vfprintf(FILE * const file, const char * restrict format, va_list args) {
	int counter;

	for (counter = 0; *format != '\0'; ++counter) {
		if (*format == '%') switch (*(++format)) {
			case '\0':
				return counter;
			case '%':
				break;
			case 'c':
				if (putc(va_arg(args, int), file) != DFLT_SUCCESS_CODE) {
					return DFLT_ERROR_CODE;
				} else {
					++format;
				} continue;
			case 'u':
				if (_putu(file, va_arg(args, unsigned)) != DFLT_SUCCESS_CODE) {
					return DFLT_ERROR_CODE;
				} else {
					++format;
				} continue;
			case 's':
				if (_puts(file, va_arg(args, char *)) != DFLT_SUCCESS_CODE) {
					return DFLT_ERROR_CODE;
				} else {
					++format;
				} continue;
			default:
				return DFLT_SUCCESS_CODE;
		}

		putc(*format++, file);
	}

	if (file == stderr) {
		if (fflush(stderr) != DFLT_SUCCESS_CODE) {
			return DFLT_ERROR_CODE;
		}
	}

	return counter;
}

int puts(const char * const str) {
	if (_puts(stdout, str) != DFLT_SUCCESS_CODE) {
		return DFLT_ERROR_CODE;
	}

	if (putchar('\n') != DFLT_SUCCESS_CODE) {
		return DFLT_ERROR_CODE;
	}

	return DFLT_SUCCESS_CODE;
}

int fprintf(FILE * const file, const char * const format, ...) {
	va_list args;
	va_start(args, format);

	const int result = vfprintf(file, format, args);

	va_end(args);
	return result;
}

int printf(const char * const format, ...) {
	va_list args;
	va_start(args, format);

	const int result = vfprintf(stdout, format, args);

	va_end(args);
	return result;
}
