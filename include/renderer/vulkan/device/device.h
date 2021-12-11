#pragma once

#include <vulkan/vulkan.h>

typedef struct {
	VkQueueFlags	require;
	VkQueueFlags	conflict;
	VkQueueFlags	desire;
	VkQueueFlags	undesire;
	VkBool32	require_surface;
	VkBool32	desire_surface;
} queue_info_t;

typedef struct {
	VkQueue		queue;
	queue_info_t	info;
	uint32_t	queue_id;
} queue_t;

typedef struct {
	VkDevice			device;
	queue_t				present_queue;
	queue_t				render_queue;
	queue_t				transfer_queue;
	queue_t				compute_queue;
	VkPhysicalDevice		physical_device;
	VkPhysicalDeviceProperties	properties;
	VkQueueFamilyProperties *	queues;
} device_t;

extern device_t main_device;

// device/vk_physical_device.c //
void vk_physical_device_init();
void vk_physical_device_release();

// device/vk_device.c //
void vk_device_create();
void vk_device_destroy();

