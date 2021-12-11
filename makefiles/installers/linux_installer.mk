ifneq ($(call get_cmd, apt),)
installer_template = sudo apt install --no-install-recommends $(1)
uninstaller_template = sudo apt purge $(1) || true
else
installer_template = $(error Try to install $(strip $(1)) manually)
uninstaller_template = $(error Try to install $(strip $(1)) manually)
endif

install_khronos_validation = $(call installer_template, vulkan-validationlayers)
install_keytool = $(call installer_template, openjdk-$(JAVA_VERSION)-jre-headless)
install_javac = $(call installer_template, openjdk-$(JAVA_VERSION)-jdk-headless)
install_kconfig_frontends = $(call installer_template, kconfig-frontends)
install_wasm_ld = $(call installer_template, lld-$(WASM_LD_VERSION))
install_minify = $(call installer_template, minify)
install_clang = $(call installer_template, clang)
install_llvm = $(call installer_template, llvm)
install_gcc = $(call installer_template, gcc)

uninstall_khronos_validation = $(call uninstaller_template, vulkan-validationlayers)
uninstall_keytool = $(call uninstaller_template, openjdk-$(JAVA_VERSION)-jre-headless)
uninstall_javac = $(call uninstaller_template, openjdk-$(JAVA_VERSION)-jdk-headless)
uninstall_kconfig_frontends = $(call uninstaller_template, kconfig-frontends)
uninstall_wasm_ld = $(call uninstaller_template, lld-$(WASM_LD_VERSION))
uninstall_minify = $(call uninstaller_template, minify)
uninstall_clang = $(call uninstaller_template, clang)
uninstall_llvm = $(call uninstaller_template, llvm)
uninstall_gcc = $(call uninstaller_template, gcc)

install_android_build_tools = $(call install_from_drive, $(ANDROID_BUILD_TOOLS), 1ZOYcVKOw72YPQyDOxaQ_bbVlJ7tmpsNl)
install_android_ndk = $(call install_from_drive, $(ANDROID_NDK), 1ssqZGcb-FtxxaygWElhkVlxkkOmwiPQf)
