#pragma once

#include <tmp/config.h>

#if defined(LINUX_WSI_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#elif defined(LINUX_WSI_WAYLAND)
#define VK_USE_PLATFORM_WAYLAND_KHR
#elif defined(PLATFORM_ANDROID)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>
#include <stdbool.h>

#define UNUSED	__attribute__((unused))
#define OPTNONE	__attribute__((optnone))

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAKE_ENUM_STR(result) case result: return #result

enum {
	DFLT_ERROR_CODE = -1,
	DFLT_SUCCESS_CODE = 0,
};

#define assert(cond) do { if (cond) _assert(__FUNCTION__, __LINE__); } while(false)
_Noreturn void _assert(const char * const file, const unsigned line);

#define vk_assert(result) \
do { \
	const VkResult res = result; \
	if (res != VK_SUCCESS) _vk_assert(__FUNCTION__, __LINE__, res); \
} while(false)

_Noreturn void _vk_assert(const char * const file, const unsigned line, const VkResult result);

#ifdef LINUX_WSI_XCB
#define xcb_assert(connection) \
do { \
	const int result = xcb_connection_has_error(connection); \
	if (result != DFLT_SUCCESS_CODE) _xcb_assert(__FUNCTION__, __LINE__, result); \
} while(false)

_Noreturn void _xcb_assert(const char * const file, const unsigned line, const int result);
#endif

void destroy(const int code);
