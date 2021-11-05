ARM64_TOOLCHAINS := $(ANDROID_TOOLCHAINS)/aarch64-linux-android-4.9/prebuilt/$(OS_ARCH)/bin
ARM64_PLATFORM_API := $(ANDROID_PLATFORM_API)/arch-arm64
ARM64_INCLUDE := $(ANDROID_INCLUDE)/aarch64-linux-android
ARM64_TARGET := $(APK_LIB_DIR)/arm64-v8a/$(APK_LIB_NAME)
ARM64_LIB_DIR := $(ARM64_PLATFORM_API)/usr/lib
ARM64_TMP_DIR := $(TMP_DIR)/arm64
ARM64_OBJCOPY := $(ARM64_TOOLCHAINS)/aarch64-linux-android-objcopy
ARM64_GCC := $(ARM64_TOOLCHAINS)/aarch64-linux-android-gcc
ANDROID_TARGET := $(ANDROID_TARGET) $(ARM64_TARGET)

include $(wildcard $(ARM64_TMP_DIR)/*.d)

ARM64_CFLAGS := $(CFLAGS) -I $(ARM64_INCLUDE)
ARM64_LDFLAGS := $(LDFLAGS) -shared -L $(ARM64_LIB_DIR) --sysroot=$(ARM64_PLATFORM_API)
ARM64_OBJECTS := $(ARM64_OBJECTS) $(call get_objects, $(ARM64_TMP_DIR), $(SOURCES))

$(ARM64_TARGET): $(ARM64_OBJECTS)
	@ $(call mkdir, $(@D))
	$(call aarch64-linux-android-gcc, $(ARM64_OBJECTS) -o $@ $(ARM64_LDFLAGS))
$(ARM64_TMP_DIR)/%.o: %.c
	@ $(call mkdir, $(@D))
	$(call aarch64-linux-android-gcc, $(ARM64_CFLAGS) -flto $< -o $@)
$(ARM64_TMP_DIR)/libc.o: libc.c
	@ $(call mkdir, $(@D))
	$(call aarch64-linux-android-gcc, $(ARM64_CFLAGS) $< -o $@)
	$(call aarch64-linux-android-objcopy, $@ $(ANDROID_OBJ_FLAGS))
