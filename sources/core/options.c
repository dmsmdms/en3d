#include <core/options.h>
#include <core/assert.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define WIDTH_MIN_VALUE 32
#define WIDTH_MAX_VALUE 4096
#define HEIGHT_MIN_VALUE 32
#define HEIGHT_MAX_VALUE 4096
#define DFLT_OPTIONS_LIST "e:f"

#ifdef LINUX_MULTI_WSI
#define LINUX_MULTI_WSI_OPT "d:"
#else
#define LINUX_MULTI_WSI_OPT
#endif

options_t options = {
	.width		= 800,
	.height		= 600,
#ifdef LINUX_MULTI_WSI
	.wsi_type	= WSI_TYPE_XCB,
#endif
	.is_fullscreen	= false,
};

void options_parse(const int argc, char * const * const argv) {
	const char options_list[] = DFLT_OPTIONS_LIST LINUX_MULTI_WSI_OPT;
	
	while (true) {
		switch (getopt(argc, argv, options_list)) {
			case 'f':
				options.is_fullscreen = true;
				break;
			case 'e': {
				const char * const restrict width_str = optarg;
				char * restrict height_str = strchr(optarg, 'x');
				assert(height_str == NULL);
				
				options.width = atoi(width_str);
				assert(options.width < WIDTH_MIN_VALUE);
				assert(options.width > WIDTH_MAX_VALUE);
				
				*height_str++ = '\0';
				options.height = atoi(height_str);
				assert(options.height < HEIGHT_MIN_VALUE);
				assert(options.height > HEIGHT_MAX_VALUE);
			} break;
#ifdef LINUX_MULTI_WSI
			case 'd':
				if(strcmp(optarg, "xcb") == DFLT_SUCCESS_CODE) {
					options.wsi_type = WSI_TYPE_XCB;
				} else if(strcmp(optarg, "wayland") == DFLT_SUCCESS_CODE) {
					options.wsi_type = WSI_TYPE_WAYLAND;
				} break;
#endif
			case DFLT_ERROR_CODE:
				return;
			default:
				assert(true);
		}
	}
}
