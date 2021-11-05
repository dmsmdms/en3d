#pragma once

enum {
	SIGSEGV,
	SIGTERM,
	SIGINT,
};

#define SIG_ERR ((__sighandler_t) -1)
typedef void (* __sighandler_t)(int);

__sighandler_t signal(const int sigid, const __sighandler_t sig_callback);
