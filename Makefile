.PHONY : all install uninstall menuconfig distclean clean
.DEFAULT_GOAL := all

BASE_BUILD_DIR := build
CONFIGS_DIR := configs
DEPENDENCIES_DIR := dependencies
INCLUDE_DIR := include
MAKEFILES_DIR := makefiles
RESOURCES_DIR := resources
SOURCES_DIR := sources

export PLATFORM_CONFIGS_DIR := $(CONFIGS_DIR)/platforms
PLATFORM_INCLUDE_DIR := $(INCLUDE_DIR)/platforms
PLATFORM_MAKEFILES_DIR := $(MAKEFILES_DIR)/platforms
PLATFORM_RESOURCES_DIR := $(RESOURCES_DIR)/platforms
PLATFORM_SOURCES_DIR := $(SOURCES_DIR)/platforms
INSTALLERS_DIR := $(MAKEFILES_DIR)/installers

include $(wildcard .config)
include $(MAKEFILES_DIR)/common_api.mk
include $(INSTALLERS_DIR)/common_installer.mk

ifneq ($(OS), Windows_NT)
OS := $(shell uname -s)
endif

PROJECT_NAME := $(call rm_quotes, $(CONFIG_PROJECT_NAME))

ifeq ($(OS), Linux)
include $(MAKEFILES_DIR)/linux_api.mk
INSTALL_DIR := $(HOME_DATA_DIR)/$(PROJECT_NAME)
include $(INSTALLERS_DIR)/linux_installer.mk
else ifeq ($(OS), Windows_NT)
include $(MAKEFILES_DIR)/windows_api.mk
INSTALL_DIR := $(HOME_DATA_DIR)/$(PROJECT_NAME)
include $(INSTALLERS_DIR)/windows_installer.mk
else
$(error $(OS) is not supported)
endif

CFLAGS := $(CFLAGS) -I $(INCLUDE_DIR) -c -MD -O3 -pipe -Wall
LDFLAGS := $(LDFLAGS)

VPATH := $(call find_nested_dir, $(SOURCES_DIR))
VPATH := $(filter-out $(call find_nested_dir, $(PLATFORM_SOURCES_DIR)), $(VPATH))

ifeq ($(CONFIG_PLATFORM_LINUX), y)
ifeq ($(OS), Linux)
include $(PLATFORM_MAKEFILES_DIR)/linux.mk
endif
else ifeq ($(CONFIG_PLATFORM_WINDOWS), y)
ifeq ($(OS), Windows_NT)
include $(PLATFORM_MAKEFILES_DIR)/windows.mk
endif
else ifeq ($(CONFIG_PLATFORM_ANDROID), y)
include $(PLATFORM_MAKEFILES_DIR)/android.mk
else ifeq ($(CONFIG_PLATFORM_WEB), y)
include $(PLATFORM_MAKEFILES_DIR)/web.mk
endif

install: $(PLATFORM_INSTALL)
	@ $(call install, kconfig-mconf, install_kconfig_frontends)
uninstall: $(PLATFORM_UNINSTALL)
	@ $(call uninstall, kconfig-mconf, uninstall_kconfig_frontends)
	@ $(if $(PROJECT_NAME), $(call rmdir, $(INSTALL_DIR)))
menuconfig:
	@ $(call kconfig-mconf, $(CONFIGS_DIR)/KConfig)
	@ $(call del, $(CONFIG_FILE))
distclean:
	$(call rmdir, $(BASE_BUILD_DIR))
	$(call del, .config.old)
	$(call del, .config)
clean:
	$(call rmdir, $(BUILD_DIR))
