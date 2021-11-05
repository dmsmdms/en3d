VPATH := $(VPATH) $(PLATFORM_SOURCES_DIR)/windows
BUILD_DIR := $(BASE_BUILD_DIR)/windows
TARGET := $(BUILD_DIR)/$(PROJECT_NAME).exe
TMP_DIR := $(BUILD_DIR)/tmp

include $(wildcard $(TMP_DIR)/*.d)

PLATFORM_INSTALL := platform_install
PLATFORM_UNINSTALL := platform_uninstall

SOURCES := $(call get_sources, $(VPATH))
OBJECTS := $(call get_objects, $(TMP_DIR), $(SOURCES))

CFLAGS := $(CFLAGS) -flto -I $(PLATFORM_INCLUDE_DIR)/windows
LDFLAGS := $(LDFLAGS) --lto

$(TARGET): $(OBJECTS)
	$(call gcc, $(OBJECTS) -o $@ $(LDFLAGS))
$(TMP_DIR)/%.o: %.c
	@ $(call mkdir, $(@D))
	$(call gcc, $(CFLAGS) $< -o $@)
$(PLATFORM_INSTALL):
	@ $(call install, gcc, install_gcc)
$(PLATFORM_UNINSTALL):
	@ $(call uninstall, gcc, uninstall_gcc)
all: $(TARGET)
