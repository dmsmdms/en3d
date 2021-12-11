#include <renderer/vulkan/device/device.h>
#include <renderer/vulkan/instance/instance.h>
#include <renderer/vulkan/surface/surface.h>
#include <core/assert.h>
#include <malloc.h>

device_t main_device = {
	.device			= VK_NULL_HANDLE,
	.present_queue		= VK_NULL_HANDLE,
	.render_queue		= VK_NULL_HANDLE,
	.transfer_queue		= VK_NULL_HANDLE,
	.compute_queue		= VK_NULL_HANDLE,
	.physical_device	= VK_NULL_HANDLE,
	.queues			= NULL,
	.queues_count		= 0,
	.present_queue_id	= UINT32_MAX,
	.render_queue_id	= UINT32_MAX,
	.transfer_queue_id	= UINT32_MAX,
	.compute_queue_id	= UINT32_MAX,
};

static uint32_t find_best_queue(const queue_requirements_t * const restrict queue) {
	
}

static VkBool32 check_physical_device(const VkPhysicalDevice physical_device) {
	uint32_t queues_count;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queues_count, NULL);
	
	VkQueueFamilyProperties queues[queues_count];
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queues_count, queues);
	
	main_device.present_queue_id = UINT32_MAX;
	main_device.render_queue_id = UINT32_MAX;
	main_device.transfer_queue_id = UINT32_MAX;
	main_device.compute_queue_id = UINT32_MAX;
	
	for(uint32_t i = 0; i < queues_count; ++i) {
		const VkQueueFlags flags = queues[i].queueFlags;
		VkBool32 has_support;
		
		vk_assert(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &has_support));
		
		if(has_support != VK_FALSE) {
			main_device.present_queue_id = i;
		}
	}
	
	return VK_FALSE;
}

void vk_physical_device_init() {
	uint32_t device_count;
	vk_assert(vkEnumeratePhysicalDevices(instance, &device_count, NULL));

	VkPhysicalDevice devices[device_count];
	vk_assert(vkEnumeratePhysicalDevices(instance, &device_count, devices));

	for(uint32_t i = 0; i < device_count; ++i) {
		const VkPhysicalDevice physical_device = devices[i];
		
		if(check_physical_device(physical_device) != VK_FALSE) {
			vkGetPhysicalDeviceProperties(physical_device, &main_device.properties);
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &main_device.queues_count, NULL);

			main_device.queues = malloc(main_device.queues_count * sizeof(main_device.queues[0]));
			assert(main_device.queues == NULL);
			
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &main_device.queues_count, main_device.queues);
			main_device.physical_device = physical_device;
		}
	}

	assert(main_device.physical_device == VK_NULL_HANDLE);
}

void vk_physical_device_release() {
	free(main_device.queues);
}
