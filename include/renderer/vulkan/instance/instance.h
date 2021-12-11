#pragma once

#include <vulkan/vulkan.h>

extern VkInstance instance;
#define VK_INIT_FUNC(func) const PFN_##func func = (PFN_##func)vkGetInstanceProcAddr(instance, #func)

void vk_instance_create();
void vk_instance_destroy();
