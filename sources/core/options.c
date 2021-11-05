#include <core/options.h>
#include <core/assert.h>
#include <getopt.h>

options_t options = {
	.is_fullscreen = false,
};

void options_parse(const int argc, char * const * const argv) {
	while (true) {
		switch (getopt(argc, argv, "f")) {
			case 'f':
				options.is_fullscreen = true;
				break;
			case DFLT_ERROR_CODE:
				return;
			default:
				assert(true);
		}
	}
}
