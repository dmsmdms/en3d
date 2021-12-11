#pragma once

#include <vulkan/vulkan.h>
#include <core/assert.h>

UNUSED static const VkAllocationCallbacks * const allocation_callbacks = NULL;
UNUSED static void vk_allocation_callbacks_init() {};
UNUSED static void vk_allocation_callbacks_destroy() {};
