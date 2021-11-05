ANDROID_DIR = $(INSTALL_DIR)/android
ANDROID_BUILD_TOOLS = $(ANDROID_DIR)/build_tools
ANDROID_SDK = $(ANDROID_DIR)/sdk
ANDROID_NDK = $(ANDROID_DIR)/ndk

install_from_drive = \
	$(call mkdir, $(1)) &&\
	$(call download, $(2), $(1).zip) &&\
	$(call unzip, $(1).zip, $(1)) &&\
	$(call del, $(1).zip)

install_android_sdk = $(call install_from_drive, $(ANDROID_SDK), 1mHmXQb5H3GscGGGPVCk1gUACzYMmwYJb)
