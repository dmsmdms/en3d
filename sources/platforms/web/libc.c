#include <core/signals.h>
#include <core/assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

extern char __heap_base;
static char * heap_base = &__heap_base;
char * restrict optarg = NULL;

static __sighandler_t sig_callbacks[] = {
	[SIGSEGV] = SIG_ERR,
	[SIGTERM] = SIG_ERR,
	[SIGINT] = SIG_ERR,
};

int main(const int argc, char * const * const argv);
extern _Noreturn void _exit(const int code);

_Noreturn void libc_init(const int argc, char * restrict argstr) {
	stdout = fdopen(STDOUT_FILENO, NULL);
	stderr = fdopen(STDERR_FILENO, NULL);

	char ** const restrict argv = malloc(argc * sizeof(argv[0]));

	for (int i = 0; i < argc; ++i) {
		argv[i] = argstr;
		argstr = strchr(argstr, ' ');
		*argstr++ = '\0';
	}

	const int code = main(argc, argv);

	free(argv);
	exit(code);
}

_Noreturn void exit(const int code) {
	fflush(stdout);
	_exit(code);
}

void * malloc(unsigned long size) {
	unsigned long * const restrict ptr = (unsigned long *)heap_base;
	size = (size + sizeof(long) - 1) & ~(sizeof(long) - 1);

	heap_base += size + sizeof(ptr[0]);
	ptr[0] = size;

	return ptr + 1;
}

void free(void * const restrict ptr) __attribute__((optnone)) {
	(void)ptr; //TODO
}

char * strchr(const char * restrict str, const int chr) {
	while (*str != '\0') {
		if (*str == chr) {
			return (char *)str;
		} else {
			++str;
		}
	}

	return NULL;
}

void * memcpy(void * restrict dst, const void * restrict src, const unsigned long size) {
	for (const void * const last_src = (char *)src + size; src < last_src; ++src, ++dst) {
		*(char *)dst = *(char *)src;
	}

	return dst - size;
}

int getopt(const int argc, char * const * const restrict argv, const char * const restrict arglist) {
	static int opt_id = 1;

	if (opt_id >= argc) {
		return DFLT_ERROR_CODE;
	}

	const char * const restrict opt = argv[opt_id++];

	if (opt[0] != '-') {
		return '?';
	}

	const char c = opt[1];
	const char * const restrict arg = strchr(arglist, c);

	if (arg == NULL) {
		fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], c);
		return '?';
	}

	if (arg[1] == ':') {
		if (opt_id < argc) {
			optarg = argv[opt_id++];
		} else if (arg[2] != ':') {
			return '?';
		} else {
			optarg = NULL;
		}
	} else {
		optarg = NULL;
	}

	return c;
}

void call_sig_callback(const int sigid) {
	const __sighandler_t callback = sig_callbacks[sigid];

	if (callback != SIG_ERR) {
		callback(sigid);
	}
}

__sighandler_t signal(const int sigid, const __sighandler_t sig_callback) {
	sig_callbacks[sigid] = sig_callback;
	return sig_callback;
}
