VPATH := $(VPATH) $(PLATFORM_SOURCES_DIR)/linux
BUILD_DIR := $(BASE_BUILD_DIR)/linux
TARGET := $(BUILD_DIR)/$(PROJECT_NAME)
TMP_DIR := $(BUILD_DIR)/tmp
CONFIG_FILE := $(TMP_DIR)/config.h

include $(wildcard $(TMP_DIR)/*.d)

PLATFORM_INSTALL := platform_install
PLATFORM_UNINSTALL := platform_uninstall

SOURCES := $(call get_sources, $(VPATH))
OBJECTS := $(call get_objects, $(TMP_DIR), $(SOURCES))

CFLAGS := $(CFLAGS) -flto -I $(PLATFORM_INCLUDE_DIR)/linux -I $(BUILD_DIR)
LDFLAGS := $(LDFLAGS) --lto -lvulkan

ifeq ($(CONFIG_LINUX_WSI_XCB), y)
LDFLAGS := $(LDFLAGS) -lxcb
endif

ifeq ($(CONFIG_LINUX_WSI_WAYLAND), y)
LDFLAGS := $(LDFLAGS)
endif

$(TARGET): $(OBJECTS)
	$(call gcc, $(OBJECTS) -o $@ $(LDFLAGS))
$(TMP_DIR)/%.o: %.c $(CONFIG_FILE)
	$(call gcc, $(CFLAGS) $< -o $@)
$(CONFIG_FILE):
	@ $(call mkdir, $(@D))
	@ $(call common_config)
$(PLATFORM_INSTALL):
	@ $(call install, gcc, install_gcc)
	@ $(if $(filter y, $(CONFIG_ENABLE_KHRONOS_VALIDATION)), $(call install_lib, $(VK_KHRONOS_VALIDATION_JSON), install_khronos_validation))
$(PLATFORM_UNINSTALL):
	@ $(if $(filter y, $(CONFIG_ENABLE_KHRONOS_VALIDATION)), $(call uninstall_lib, $(VK_KHRONOS_VALIDATION_JSON), uninstall_khronos_validation))
	@ $(call uninstall, gcc, uninstall_gcc)
all: $(TARGET)
