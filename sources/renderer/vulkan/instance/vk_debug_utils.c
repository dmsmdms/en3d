#include <renderer/vulkan/instance/debug_utils.h>
#include <renderer/vulkan/allocation_callbacks.h>
#include <renderer/vulkan/instance/instance.h>
#include <core/assert.h>
#include <stdio.h>

#ifdef ENABLE_DEBUG_UTILS
static const char * get_message_severity_str(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
	switch (messageSeverity) {
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT);
	}

	return NULL;
}

static const char * get_message_type_str(const VkDebugUtilsMessageTypeFlagsEXT messageTypes) {
	switch (messageTypes) {
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT);
		MAKE_ENUM_STR(VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
	}

	return NULL;
}

static VkBool32 debug_messenger_callback(
	const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT * const restrict pCallbackData,
	void * const pUserData UNUSED)
{
	static const char * const format = "[%s]";
	printf(format, get_message_severity_str(messageSeverity));

	for (VkFlags i = 1; messageTypes != 0; messageTypes >>= 1, i <<= 1) {
		if (messageTypes & VK_TRUE) {
			printf(format, get_message_type_str(i));
		}
	}

	puts(pCallbackData->pMessage);
	fflush(stdout);

	return VK_FALSE;
}

static const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {
	.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
	.pNext			= NULL,
	.flags			= 0,
	.messageSeverity	= (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT),
	.messageType		= (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT),
	.pfnUserCallback	= debug_messenger_callback,
	.pUserData		= NULL,
};

const VkDebugUtilsMessengerCreateInfoEXT * const debug_messenger_create_info_ptr = &debug_messenger_create_info;
static VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

void vk_debug_utils_create() {
	VK_INIT_FUNC(vkCreateDebugUtilsMessengerEXT);
	
	assert(vkCreateDebugUtilsMessengerEXT == NULL);
	vk_assert(vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_create_info, allocation_callbacks, &debug_messenger));
}

void vk_debug_utils_destroy() {
	VK_INIT_FUNC(vkDestroyDebugUtilsMessengerEXT);

	if (vkDestroyDebugUtilsMessengerEXT != NULL) {
		vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, allocation_callbacks);
	}
}
#endif
