VP=Engine/Vendor
BUILD_DIR?=Bin

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,$(VP)/YAML/src/,*.cpp)

.PHONY: All

All:
	g++ -g -shared -I$(VP)/YAML/include -L$(BUILD_DIR) $(SRC_FILES) -o $(VP)/YAML/yaml.dll
	cp $(VP)/YAML/yaml.dll $(BUILD_DIR)/yaml.dll
