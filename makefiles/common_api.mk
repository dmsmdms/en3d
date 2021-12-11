rename = mv -f $(1) $(2)
mkdir = mkdir -p $(1)
rmdir = rm -rf $(1)
del = rm -f $(1)

replace_file_str = sed "s|$(strip $(1))|$(strip $(2))|g" $(3) > $(4)
find_nested_dir = $(shell $(call find, $(1) -type d))
get_cookies = $(shell wget -q --save-cookies $(HOME_DATA_DIR)/$(strip $(1)).txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=$(strip $(1))' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')
download = wget --no-check-certificate --load-cookies $(HOME_DATA_DIR)/$(strip $(1)).txt "https://docs.google.com/uc?export=download&confirm=$(call get_cookies, $(1))&id=$(strip $(1))" -O $(2) && $(call del, $(HOME_DATA_DIR)/$(strip $(1)).txt)
unzip = unzip -o -q $(1) -d $(2)

replace_project_name = $(call replace_file_str, @PROJECT_NAME@, $(PROJECT_NAME), $(1), $(2))
rm_quotes = $(patsubst "%",%, $(1))

install_android = $(if $(wildcard $(1)), $(info [OK] $(1)), $(call $(2)))
install_wasm32 = $(if $(get_wasm32_support), $(info [OK]  wasm32), $(call $(1)))
uninstall_wasm32 = $(if $(get_wasm32_support), $(call $(1)), $(info [NOT SUPPORTED]  wasm32))

get_resources = $(addprefix $(1)/, $(subst $(strip $(2)/),, $(wildcard $(2)/$(strip $(3)))))
get_java_sources = $(notdir $(wildcard $(addsuffix /*.java, $(1))))
get_java_objects = $(addprefix $(1)/, $(patsubst %.java, %.class, $(2)))
get_sources = $(notdir $(wildcard $(addsuffix /*.c, $(1))))
get_objects = $(addprefix $(1)/, $(patsubst %.c, %.o, $(2)))

common_config = \
	$(call add_config, "\#pragma once") &&\
	$(if $(filter y, $(CONFIG_PLATFORM_LINUX)), $(call add_config, "\#define PLATFORM_LINUX") &&) \
	$(if $(filter y, $(CONFIG_PLATFORM_ANDROID)), $(call add_config, "\#define PLATFORM_ANDROID") &&) \
	$(if $(filter y, $(CONFIG_PLATFORM_WEB)), $(call add_config, "\#define PLATFORM_WEB") &&) \
	$(if $(filter y, $(CONFIG_ENABLE_LUNARG_STANDART_VALIDATION)), $(call add_config, "\#define ENABLE_LUNARG_STANDART_VALIDATION") &&) \
	$(if $(filter y, $(CONFIG_ENABLE_KHRONOS_VALIDATION)), $(call add_config, "\#define ENABLE_KHRONOS_VALIDATION") &&) \
	$(if $(filter y, $(CONFIG_ENABLE_DEBUG_UTILS)), $(call add_config, "\#define ENABLE_DEBUG_UTILS") &&) \
	$(if $(filter y, $(CONFIG_LINUX_WSI_XCB)), $(call add_config, "\#define LINUX_WSI_XCB") &&) \
	$(if $(filter y, $(CONFIG_LINUX_WSI_WAYLAND)), $(call add_config, "\#define LINUX_WSI_WAYLAND") &&) \
	$(if $(CONFIG_WEB_FSTREAM_BUF_SIZE), $(call add_config, "\#define WEB_FSTREAM_BUF_SIZE $(CONFIG_WEB_FSTREAM_BUF_SIZE)") &&) \
	$(call add_config, "\#define PROJECT_NAME \"$(CONFIG_PROJECT_NAME)\"") &&\
	$(call add_config, "\#define PROJECT_VERSION_MAJOR $(CONFIG_PROJECT_VERSION_MAJOR)") &&\
	$(call add_config, "\#define PROJECT_VERSION_MINOR $(CONFIG_PROJECT_VERSION_MINOR)") &&\
	$(call add_config, "\#define PROJECT_VERSION_PATCH $(CONFIG_PROJECT_VERSION_PATCH)") &&\
	$(call add_config, "\#define ENGINE_NAME \"$(CONFIG_ENGINE_NAME)\"") &&\
	$(call add_config, "\#define ENGINE_VERSION_MAJOR $(CONFIG_ENGINE_VERSION_MAJOR)") &&\
	$(call add_config, "\#define ENGINE_VERSION_MINOR $(CONFIG_ENGINE_VERSION_MINOR)") &&\
	$(call add_config, "\#define ENGINE_VERSION_PATCH $(CONFIG_ENGINE_VERSION_PATCH)")
