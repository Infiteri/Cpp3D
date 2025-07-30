VP=Engine/Vendor
BUILD_DIR?=Bin

SRC_FILES := $(VP)/glad/src/glad.c

.PHONY: All
All:
	g++ -g -shared -I$(VP)/glad/include -L$(BUILD_DIR) $(SRC_FILES) -o $(VP)/glad/lib/glad.dll
	cp $(VP)/glad/lib/glad.dll $(BUILD_DIR)/glad.dll
