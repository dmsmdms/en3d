#include <renderer/renderer.h>
#include <core/signals.h>
#include <core/options.h>
#include <core/assert.h>

void destroy(const int code UNUSED) {
	renderer_destroy();
}

int main(const int argc, char * const * const argv) {
	// Core Init //
	signals_set_callbacks();
	options_parse(argc, argv);
	
	// Renderer Init //
	renderer_init();

	// Destroy All //
	destroy(DFLT_SUCCESS_CODE);
	return DFLT_SUCCESS_CODE;
}
