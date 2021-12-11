#include <core/options.h>
#include <core/assert.h>
#include <window.h>

#include <renderer/vulkan/surface/surface.h>
#include <renderer/vulkan/instance/instance.h>
#include <renderer/vulkan/allocation_callbacks.h>

VkSurfaceKHR surface = VK_NULL_HANDLE;

#ifdef LINUX_WSI_XCB
static void xcb_surface_create() {
	const VkXcbSurfaceCreateInfoKHR surface_create_info = {
		.sType		= VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
		.pNext		= NULL,
		.flags		= 0,
		.connection	= window.connection,
		.window		= window.window,
	};
	
	vk_assert(vkCreateXcbSurfaceKHR(instance, &surface_create_info, allocation_callbacks, &surface));
}
#endif

#ifdef LINUX_WSI_WAYLAND
static void wayland_surface_create() {

}
#endif

void vk_surface_create() {
#if defined(LINUX_MULTI_WSI)
	switch(options.wsi_type) {
		case WSI_TYPE_XCB:
			xcb_surface_create();
		break;
		case WSI_TYPE_WAYLAND:
			wayland_surface_create();
		break;
	}
#elif defined(LINUX_WSI_XCB)
	xcb_surface_create();
#elif defined(LINUX_WSI_WAYLAND)
	wayland_surface_create();
#endif
}

void vk_surface_destroy() {
	if(surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance, surface, allocation_callbacks);
	}
}
