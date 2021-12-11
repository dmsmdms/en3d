#pragma once

#include <tmp/config.h>

#ifdef LINUX_WSI_XCB
#include <xcb/xcb.h>
#endif

typedef struct {
#ifdef LINUX_WSI_XCB
	xcb_connection_t *	connection;
	xcb_window_t		window;
	xcb_atom_t		atom_delete_window;
#endif	
} window_t;

extern window_t window;

void window_create();
void window_destroy();
