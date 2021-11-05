aarch64-linux-android-objcopy = $(ARM64_OBJCOPY).exe $(1)
aarch64-linux-android-gcc = $(ARM64_GCC).exe $(1)
apksigner = $(ANDROID_APKSIGNER).bat $(1)
aapt = $(ANDROID_AAPT).exe $(1)
dx = $(ANDROID_DX).bat $(1)

get_cmd = $(shell where $(1))
unix_path = $(strip $(subst \,/,$(1)))
win_path = $(strip $(subst /,\,$(1)))
find = $(MSYS_BIN)\find $(1)

MSYS_BIN := $(patsubst %\bin\mingw32-make.exe, %\msys\1.0\bin, $(call get_cmd, mingw32-make))
OS_ARCH := windows-$(if $(filter 32-bit, $(shell wmic OS get OSArchitecture)),x86,x86_64)
HOME_DATA_DIR := $(call unix_path, $(LOCALAPPDATA))
WASM_LD := wasm-ld

kconfig-mconf = $(KCONFIG_FRONTENDS_DIR)/kconfig-mconf.exe $(1)
wasm-ld = $(LLVM_DIR)/bin/wasm-ld.exe $(1)
minify =  $(MINIFY_DIR)/minify.exe $(1)
clang = $(LLVM_DIR)/bin/clang.exe $(1)
keytool = $(JDK_DIR)/keytool.exe $(1)
javac = $(JDK_DIR)/javac.exe $(1)
gcc = gcc.exe $(1)

get_wasm32_support = $(if $(wildcard $(call wasm-ld)), wasm32)
install = $(if $(wildcard $(call $(1))), $(info [OK] $(1)), $(call $(2)))
uninstall = $(if $(wildcard $(call $(1))), $(call $(2)), $(info [NOT FOUND] $(1)))
