#pragma once

#include <tmp/config.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(LINUX_WSI_XCB) && defined(LINUX_WSI_WAYLAND)
#define LINUX_MULTI_WSI
#endif

#ifdef LINUX_MULTI_WSI
typedef enum {
	WSI_TYPE_WAYLAND,
	WSI_TYPE_XCB,
} wsi_type_t;
#endif

typedef struct {
	uint32_t width;
	uint32_t height;
#ifdef LINUX_MULTI_WSI
	wsi_type_t wsi_type;
#endif
	bool is_fullscreen;
} options_t;

extern options_t options;
void options_parse(const int argc, char * const * const argv);
