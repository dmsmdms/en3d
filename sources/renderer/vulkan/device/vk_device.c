#include <renderer/vulkan/device/device.h>
#include <renderer/vulkan/allocation_callbacks.h>

void vk_device_create() {
	static const char * const device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
	
	static const float queue_priorities[] = {
		[0] = 1.0,
	};
	
	const VkDeviceQueueCreateInfo device_queue_create_info[] = {
		{
			.sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext			= NULL,
			.flags			= 0,
			.queueFamilyIndex	= main_device.present_queue_id,
			.queueCount		= ARRAY_LEN(queue_priorities),
			.pQueuePriorities	= queue_priorities,	
		},
		{
			.sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext			= NULL,
			.flags			= 0,
			.queueFamilyIndex	= main_device.render_queue_id,
			.queueCount		= ARRAY_LEN(queue_priorities),
			.pQueuePriorities	= queue_priorities,	
		},
	};
	
	const VkBool32 present_eq_render_queue = main_device.present_queue_id == main_device.render_queue_id;
	const uint32_t device_queue_count = present_eq_render_queue ? 1 : 2;
	
	const VkDeviceCreateInfo device_create_info = {
		.sType				= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext				= NULL,
		.flags				= 0,
		.queueCreateInfoCount		= device_queue_count,
		.pQueueCreateInfos		= device_queue_create_info,
		.enabledLayerCount		= 0,
		.ppEnabledLayerNames		= NULL,
		.enabledExtensionCount		= ARRAY_LEN(device_extensions),
		.ppEnabledExtensionNames	= device_extensions,
		.pEnabledFeatures		= NULL,
	};
	
	vk_assert(vkCreateDevice(main_device.physical_device, &device_create_info, allocation_callbacks, &main_device.device));
}

void vk_device_destroy() {
	vkDestroyDevice(main_device.device, allocation_callbacks);
}
