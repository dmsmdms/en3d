#include <renderer/vulkan/allocation_callbacks.h>
#include <renderer/vulkan/instance/instance.h>
#include <renderer/vulkan/instance/debug_utils.h>
#include <renderer/vulkan/device/device.h>
#include <renderer/vulkan/surface/surface.h>
#include <renderer/renderer.h>
#include <window.h>

void renderer_init() {
	vk_allocation_callbacks_init();
	vk_instance_create();
	vk_debug_utils_create();
	window_create();
	vk_surface_create();
	vk_physical_device_init();
}

void renderer_destroy() {
	vk_physical_device_release();
	vk_surface_destroy();
	window_destroy();
	vk_debug_utils_destroy();
	vk_instance_destroy();
	vk_allocation_callbacks_destroy();
}
