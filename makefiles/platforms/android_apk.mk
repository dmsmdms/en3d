ANDROID_RESOURCES := $(PLATFORM_RESOURCES_DIR)/android
ANDROID_TMP_RESOURCES := $(TMP_DIR)/res

.PRECIOUS: $(ANDROID_TMP_RESOURCES)/%

APK_UNSIGNED_PATH := $(TMP_DIR)/$(PROJECT_NAME).apk
APK_KEY_PATH := $(TMP_DIR)/$(PROJECT_NAME).jks
APK_LIB_NAME := lib$(PROJECT_NAME).so
APK_DIR := $(TMP_DIR)/apk

CONFIG_APK_KEY_CN := $(call rm_quotes, $(CONFIG_APK_KEY_CN))
CONFIG_APK_KEY_OU := $(call rm_quotes, $(CONFIG_APK_KEY_OU))
CONFIG_APK_KEY_O := $(call rm_quotes, $(CONFIG_APK_KEY_O))
CONFIG_APK_KEY_L := $(call rm_quotes, $(CONFIG_APK_KEY_L))
CONFIG_APK_KEY_ST := $(call rm_quotes, $(CONFIG_APK_KEY_ST))
CONFIG_APK_KEY_C := $(call rm_quotes, $(CONFIG_APK_KEY_C))
CONFIG_APK_KEY_PASSWORD := $(call rm_quotes, $(CONFIG_APK_KEY_PASSWORD))

APK_LIB_DIR := $(APK_DIR)/lib
APK_DEX_NAME := $(APK_DIR)/classes.dex
APK_KEY_INFO := CN=$(CONFIG_APK_KEY_CN),OU=$(CONFIG_APK_KEY_OU),O=$(CONFIG_APK_KEY_O),L=$(CONFIG_APK_KEY_L),ST=$(CONFIG_APK_KEY_ST),C=$(CONFIG_APK_KEY_C)
APK_KEY_FLAGS := -genkey -dname $(APK_KEY_INFO) -keypass $(CONFIG_APK_KEY_PASSWORD) -storepass $(CONFIG_APK_KEY_PASSWORD) -keyalg RSA -keysize 2048
ANDROID_APK_FLAGS := sign --ks-pass pass:$(CONFIG_APK_KEY_PASSWORD) --ks $(APK_KEY_PATH)

JAVA_DIR := $(TMP_DIR)/java
JAVA_CLASS_DIR := $(JAVA_DIR)/com/$(PROJECT_NAME)
JAVA_SOURCES := $(call get_java_sources, $(ANDROID_RESOURCES)/java)
JAVA_OBJECTS := $(call get_java_objects, $(JAVA_CLASS_DIR), $(JAVA_SOURCES))

$(APK_KEY_PATH):
	@ $(call mkdir, $(@D))
	$(call keytool, $(APK_KEY_FLAGS) -keystore $@)
$(APK_DEX_NAME): $(JAVA_OBJECTS)
	@ $(call mkdir, $(@D))
	$(call dx, --dex --output $@ $(JAVA_DIR))
$(JAVA_CLASS_DIR)/%.class: $(ANDROID_TMP_RESOURCES)/java/%.java $(ANDROID_JAR)
	@ $(call mkdir, $(@D))
	$(call javac, -cp $(ANDROID_JAR) -Xlint:-options --release 7 -d $(JAVA_DIR) $<)
$(ANDROID_TMP_RESOURCES)/%: $(ANDROID_RESOURCES)/%
	@ $(call mkdir, $(@D))
	$(call replace_project_name, $<, $@)
