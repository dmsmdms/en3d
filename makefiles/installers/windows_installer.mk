KCONFIG_FRONTENDS_DIR := $(HOME_DATA_DIR)/kconfig_frontends
export JAVA_HOME := $(INSTALL_DIR)/android/jdk
MINIFY_DIR := $(INSTALL_DIR)/web/minify
LLVM_DIR := $(INSTALL_DIR)/web/llvm
JDK_DIR := $(JAVA_HOME)/bin

install_kconfig_frontends = $(call install_from_drive, $(KCONFIG_FRONTENDS_DIR), 1KgkDldp8m0IgAFsQckH-CktfqRd45KBc)
install_minify = $(call install_from_drive, $(MINIFY_DIR), 1ttaHkwJ2exHWZrgutuhZE8ysfIoY17dY)
install_clang = $(call install_from_drive, $(LLVM_DIR), 14mcugly51v7-3ddpRZxuYas45n_He6dx)

uninstall_kconfig_frontends = $(call rmdir, $(KCONFIG_FRONTENDS_DIR))
uninstall_minify = $(call rmdir, $(MINIFY_DIR))
uninstall_clang = $(call rmdir, $(LLVM_DIR))

install_android_build_tools = $(call install_from_drive, $(ANDROID_BUILD_TOOLS), 1uopdylDyJKtr5JUGApTqxKFZmAWvB7aH)
install_android_ndk = $(call install_from_drive, $(ANDROID_NDK), 1fvwKycY-VD9IwM2qYOksXSMLhAeSYpP0)
install_keytool = $(call install_from_drive, $(JAVA_HOME), 1sw4NE86xySkDEDD566lDwQlzLMyC8g5C)
