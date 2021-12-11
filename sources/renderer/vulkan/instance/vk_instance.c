#include <core/assert.h>
#include <tmp/config.h>

#include <renderer/vulkan/instance/instance.h>
#include <renderer/vulkan/allocation_callbacks.h>
#include <renderer/vulkan/instance/debug_utils.h>

VkInstance instance = VK_NULL_HANDLE;

void vk_instance_create() {
	const uint32_t project_version = VK_MAKE_VERSION(
		PROJECT_VERSION_MAJOR,
		PROJECT_VERSION_MINOR,
		PROJECT_VERSION_PATCH
	);

	const uint32_t engine_version = VK_MAKE_VERSION(
		ENGINE_VERSION_MAJOR,
		ENGINE_VERSION_MINOR,
		ENGINE_VERSION_PATCH
	);

	static const char * const enabled_layers[] = {
#ifdef ENABLE_LUNARG_STANDART_VALIDATION
		"VK_LAYER_LUNARG_standard_validation",
#endif
#ifdef ENABLE_KHRONOS_VALIDATION
		"VK_LAYER_KHRONOS_validation",
#endif
	};

	static const char * const enabled_extensions[] = {
#ifdef VK_USE_PLATFORM_XCB_KHR
		VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#ifdef ENABLE_DEBUG_UTILS
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
	};

	const VkApplicationInfo application_info = {
		.sType			= VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext			= NULL,
		.pApplicationName	= PROJECT_NAME,
		.applicationVersion	= project_version,
		.pEngineName		= ENGINE_NAME,
		.engineVersion		= engine_version,
		.apiVersion		= VK_VERSION_1_0,
	};

	const VkInstanceCreateInfo instance_create_info = {
		.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext				= debug_messenger_create_info_ptr,
		.flags				= 0,
		.pApplicationInfo		= &application_info,
		.enabledLayerCount		= ARRAY_LEN(enabled_layers),
		.ppEnabledLayerNames		= enabled_layers,
		.enabledExtensionCount		= ARRAY_LEN(enabled_extensions),
		.ppEnabledExtensionNames	= enabled_extensions,
	};

	vk_assert(vkCreateInstance(&instance_create_info, allocation_callbacks, &instance));
}

void vk_instance_destroy() {
	vkDestroyInstance(instance, allocation_callbacks);
}
