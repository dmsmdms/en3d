VPATH := $(VPATH) $(PLATFORM_SOURCES_DIR)/android
BUILD_DIR := $(BASE_BUILD_DIR)/android
TARGET := $(BUILD_DIR)/$(PROJECT_NAME).apk
TMP_DIR := $(BUILD_DIR)/tmp

include $(PLATFORM_MAKEFILES_DIR)/android_apk.mk

PLATFORM_INSTALL := platform_install
PLATFORM_UNINSTALL := platform_uninstall

ANDROID_PLATFORM_API := $(ANDROID_NDK)/platforms/android-24
ANDROID_APKSIGNER := $(ANDROID_BUILD_TOOLS)/apksigner
ANDROID_INCLUDE := $(ANDROID_NDK)/sysroot/usr/include
ANDROID_TOOLCHAINS := $(ANDROID_NDK)/toolchains
ANDROID_AAPT := $(ANDROID_BUILD_TOOLS)/aapt
ANDROID_JAR := $(ANDROID_SDK)/android.jar
ANDROID_DX := $(ANDROID_BUILD_TOOLS)/dx

CFLAGS := $(CFLAGS) -Wno-attributes -std=c11 -I $(PLATFORM_INCLUDE_DIR)/android -I $(ANDROID_INCLUDE)
LDFLAGS := $(LDFLAGS) --lto -llog

ANDROID_OBJ_FLAGS := --redefine-sym startModule=Java_com_$(PROJECT_NAME)_MainActivity_startModule
SOURCES := $(call get_sources, $(VPATH))

ifeq ($(CONFIG_ANDROID_ARCH_ARM64), y)
include $(PLATFORM_MAKEFILES_DIR)/android_arm64.mk
endif

$(TARGET): $(APK_UNSIGNED_PATH) $(APK_KEY_PATH)
	$(call apksigner, $(ANDROID_APK_FLAGS) --out $@ --in $<)
$(APK_UNSIGNED_PATH): $(ANDROID_TMP_RESOURCES)/AndroidManifest.xml $(ANDROID_TARGET) $(APK_DEX_NAME)
	$(call aapt, package -fM $< -F $@ -I $(ANDROID_JAR) $(APK_DIR))
$(PLATFORM_INSTALL):
	@ $(call install, javac, install_javac)
	@ $(call install, keytool, install_keytool)
	@ $(call install_android, $(ANDROID_SDK), install_android_sdk)
	@ $(call install_android, $(ANDROID_NDK), install_android_ndk)
	@ $(call install_android, $(ANDROID_BUILD_TOOLS), install_android_build_tools)
$(PLATFORM_UNINSTALL):
	@ $(call uninstall, keytool, uninstall_keytool)
	@ $(call uninstall, javac, uninstall_javac)
	@ $(call rmdir, $(ANDROID_INSTALL_DIR))
all: $(TARGET)
