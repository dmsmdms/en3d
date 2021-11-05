WEB_RESOURCES_MINIFY := $(filter y, $(CONFIG_WEB_RESOURCES_MINIFY))
VPATH := $(VPATH) $(PLATFORM_SOURCES_DIR)/web
BUILD_DIR := $(BASE_BUILD_DIR)/web
RESOURCES_DIR := $(PLATFORM_RESOURCES_DIR)/web
TARGET := $(BUILD_DIR)/$(PROJECT_NAME).wasm
TMP_DIR := $(BUILD_DIR)/tmp

include $(wildcard $(TMP_DIR)/*.d)

PLATFORM_INSTALL := platform_install
PLATFORM_UNINSTALL := platform_uninstall

SOURCES := $(call get_sources, $(VPATH))
OBJECTS := $(call get_objects, $(TMP_DIR), $(SOURCES))

RESOURCES := $(RESOURCES) $(call get_resources, $(BUILD_DIR), $(RESOURCES_DIR), *.html)
RESOURCES := $(RESOURCES) $(call get_resources, $(BUILD_DIR), $(RESOURCES_DIR), js/*.js)

CFLAGS := $(CFLAGS) -flto --target=wasm32 -I $(PLATFORM_INCLUDE_DIR)/web -D WEB_FSTREAM_BUF_SIZE=$(CONFIG_WEB_FSTREAM_BUF_SIZE)
LDFLAGS := $(LDFLAGS) --lto-O3 --no-entry --allow-undefined --export=libc_init --export=malloc --export=free --export=call_sig_callback

$(TARGET): $(OBJECTS)
	$(call wasm-ld, $(OBJECTS) -o $@ $(LDFLAGS))
$(TMP_DIR)/%.o: %.c
	@ $(call mkdir, $(@D))
	$(call clang, $(CFLAGS) $< -o $@)
$(BUILD_DIR)/%: $(RESOURCES_DIR)/%
	@ $(call mkdir, $(@D))
	$(call replace_project_name, $<, $@)
	$(if $(WEB_RESOURCES_MINIFY), $(call minify, $@ -o $@))
$(PLATFORM_INSTALL):
	@ $(call install, clang, install_clang)
	@ $(call install_wasm32, install_llvm)
	@ $(call install, $(WASM_LD), install_wasm_ld)
	@ $(if $(WEB_RESOURCES_MINIFY), $(call install, minify, install_minify))
$(PLATFORM_UNINSTALL):
	@ $(call uninstall, clang, uninstall_clang)
	@ $(call uninstall_wasm32, uninstall_llvm)
	@ $(call uninstall, $(WASM_LD), uninstall_wasm_ld)
	@ $(if $(WEB_RESOURCES_MINIFY), $(call uninstall, minify, uninstall_minify))
all: $(TARGET) $(RESOURCES)
