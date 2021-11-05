#include <core/signals.h>
#include <core/assert.h>
#include <signal.h>

static void dflt_signal_callback(const int code) {
	switch (code) {
		case SIGSEGV:
			assert(true);
		case SIGTERM:
			assert(true);
		case SIGINT:
			assert(true);
	}
}

void signals_set_callbacks(void) {
	assert(signal(SIGSEGV, dflt_signal_callback) == SIG_ERR);
	assert(signal(SIGTERM, dflt_signal_callback) == SIG_ERR);
	assert(signal(SIGINT, dflt_signal_callback) == SIG_ERR);
}
