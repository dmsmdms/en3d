#pragma once

#include <vulkan/vulkan.h>
#include <core/assert.h>
#include <tmp/config.h>

#ifdef ENABLE_DEBUG_UTILS
extern const VkDebugUtilsMessengerCreateInfoEXT * const debug_messenger_create_info_ptr;

void vk_debug_utils_create();
void vk_debug_utils_destroy();
#else
UNUSED static const void * const debug_messenger_create_info_ptr = NULL;

UNUSED static void vk_debug_utils_create() {}
UNUSED static void vk_debug_utils_destroy() {}
#endif
