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

get_resources = $(addprefix $(1)/, $(subst $(2)/,, $(wildcard $(2)/$(strip $(3)))))
get_java_sources = $(notdir $(wildcard $(addsuffix /*.java, $(1))))
get_java_objects = $(addprefix $(1)/, $(patsubst %.java, %.class, $(2)))
get_sources = $(notdir $(wildcard $(addsuffix /*.c, $(1))))
get_objects = $(addprefix $(1)/, $(patsubst %.c, %.o, $(2)))
