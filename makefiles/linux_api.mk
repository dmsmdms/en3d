VK_KHRONOS_VALIDATION_JSON := /usr/share/vulkan/explicit_layer.d/VkLayer_khronos_validation.json

aarch64-linux-android-objcopy = $(ARM64_OBJCOPY) $(1)
aarch64-linux-android-gcc = $(ARM64_GCC) $(1)
apksigner = $(ANDROID_APKSIGNER) $(1)
aapt = $(ANDROID_AAPT) $(1)
dx = $(ANDROID_DX) $(1)

add_config = echo $(1) >> $(CONFIG_FILE)
get_cmd = $(shell which $(1))
find = find $(1)
gcc = gcc $(1)

OS_ARCH := linux-$(shell uname -m)
HOME_DATA_DIR := $(HOME)/.local/share

WASM_LD_VERSION := 10
JAVA_VERSION := 17

WASM_LD := wasm-ld-$(WASM_LD_VERSION)
kconfig-mconf = kconfig-mconf $(1)
wasm-ld = $(WASM_LD) $(1)
keytool = keytool $(1)
minify = minify $(1)
javac = javac $(1)
clang = clang $(1)

get_wasm32_support = $(filter wasm32, $(shell llc --version))
install_lib = $(if $(wildcard $(1)), $(info [OK] $(1)), $(call $(2)))
uninstall_lib = $(if $(wildcard $(1)), $(call $(2)), $(info [NOT FOUND] $(1)))
install = $(if $(call get_cmd, $(1)), $(info [OK] $(1)), $(call $(2)))
uninstall = $(if $(call get_cmd, $(1)), $(call $(2)), $(info [NOT FOUND] $(1)))
