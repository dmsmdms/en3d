#include <core/signals.h>
#include <core/options.h>
#include <core/assert.h>
#include <stdio.h>

int main(const int argc, char * const * const argv) {
	signals_set_callbacks();
	options_parse(argc, argv);

	printf("FS: %u\n", options.is_fullscreen);

	return DFLT_SUCCESS_CODE;
}
