VP=Engine/Vendor
BUILD_DIR?=Bin

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,$(VP)/ImGui/,*.cpp)

.PHONY: All

All:
	g++ -g -shared -I$(VP)/ImGui -I$(VP)/GLFW/include -L$(BUILD_DIR) -lglfw3 $(SRC_FILES) -o $(VP)/ImGui/ImGui.dll
	cp $(VP)/ImGui/ImGui.dll $(BUILD_DIR)/ImGui.dll
