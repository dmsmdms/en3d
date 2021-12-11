#include <core/options.h>
#include <core/assert.h>
#include <window.h>
#include <malloc.h>

window_t window = {
#ifdef LINUX_WSI_XCB
	.connection	= NULL,
	.window		= 0,
#endif
#ifdef LINUX_WSI_WAYLAND
#endif
};

#ifdef LINUX_WSI_XCB
#define ATOM_HELPER(str, if_exists) atom_helper(str, ARRAY_LEN(str), if_exists)

static xcb_intern_atom_reply_t * atom_helper(const char * const str, const uint32_t length, const bool if_exists) {
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(window.connection, if_exists, length, str);
	return xcb_intern_atom_reply(window.connection, cookie, NULL);
}

static void xcb_window_create() {
	int screen_id;
	
	window.connection = xcb_connect(NULL, &screen_id);
	xcb_assert(window.connection);
	
	const xcb_setup_t * const setup = xcb_get_setup(window.connection);
	xcb_screen_iterator_t screen_iterator;
	
	for(screen_iterator = xcb_setup_roots_iterator(setup); screen_id > 0; --screen_id) {
		xcb_screen_next(&screen_iterator);
	}
	
	const xcb_screen_t * const restrict screen = screen_iterator.data;
	window.window = xcb_generate_id(window.connection);
	
	const uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	const uint32_t value_list[] = {
		screen->black_pixel,
		XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_KEY_PRESS |
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		XCB_EVENT_MASK_POINTER_MOTION |
		XCB_EVENT_MASK_BUTTON_PRESS |
		XCB_EVENT_MASK_BUTTON_RELEASE
	};
	
	if(options.is_fullscreen == true) {
		options.width = screen->width_in_pixels;
		options.height = screen->height_in_pixels;
	}
	
	xcb_create_window(window.connection, XCB_COPY_FROM_PARENT, window.window, screen->root, 0, 0,
		options.width, options.width, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
		screen->root_visual, value_mask, value_list);
	
	xcb_intern_atom_reply_t * const restrict atom_protocols = ATOM_HELPER("WM_PROTOCOLS", true);
	xcb_intern_atom_reply_t * const restrict atom_delete_window = ATOM_HELPER("WM_DELETE_WINDOW", false);
	
	xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE, window.window, atom_protocols->atom, 4,
		sizeof(atom_delete_window->atom) * 8, 1, &atom_delete_window->atom);
	window.atom_delete_window = atom_delete_window->atom;
	
	xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE, window.window, XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING, sizeof(char) * 8, ARRAY_LEN(PROJECT_NAME), PROJECT_NAME);
	
	free(atom_delete_window);
	free(atom_protocols);
	
	const char wm_class[] = PROJECT_NAME "\0" PROJECT_NAME;
	xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE, window.window, XCB_ATOM_WM_CLASS,
		XCB_ATOM_STRING, sizeof(char) * 8, sizeof(wm_class), wm_class);
	
	if(options.is_fullscreen == true) {
		xcb_intern_atom_reply_t * const restrict atom_wm_state = ATOM_HELPER("_NET_WM_STATE", false);
		xcb_intern_atom_reply_t * const restrict atom_wm_fullscreen = ATOM_HELPER("_NET_WM_STATE_FULLSCREEN", false);
		
		xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE, window.window, atom_wm_state->atom,
			XCB_ATOM_ATOM, sizeof(atom_wm_fullscreen->atom) * 8, 1, &atom_wm_fullscreen->atom);
		
		free(atom_wm_fullscreen);
		free(atom_wm_state);
	}
	
	xcb_map_window(window.connection, window.window);
}

static void xcb_window_destroy() {
	xcb_destroy_window(window.connection, window.window);
	xcb_disconnect(window.connection);
}
#endif

#ifdef LINUX_WSI_WAYLAND
static void wayland_window_create() {

}

static void wayland_window_destroy() {

}
#endif

void window_create() {
#if defined(LINUX_MULTI_WSI)
	switch(options.wsi_type) {
		case WSI_TYPE_XCB:
			xcb_window_create();
		break;
		case WSI_TYPE_WAYLAND:
			wayland_window_create();
		break;
	}
#elif defined(LINUX_WSI_XCB)
	xcb_window_create();
#elif defined(LINUX_WSI_WAYLAND)
	wayland_window_create();
#endif
}

void window_destroy() {
#if defined(LINUX_MULTI_WSI)
	switch(options.wsi_type) {
		case WSI_TYPE_XCB:
			xcb_window_destroy();
		break;
		case WSI_TYPE_WAYLAND:
			wayland_window_destroy();
		break;
	}
#elif defined(LINUX_WSI_XCB)
	xcb_window_destroy();
#elif defined(LINUX_WSI_WAYLAND)
	wayland_window_destroy();
#endif
}
