VP=Engine/Vendor
BUILD_DIR?=Bin

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,$(VP)/ImGuizmo/,*.cpp)

.PHONY: All

All:
	g++ -g -shared -I$(VP)/ImGui -I$(VP)/ImGuizmo -L$(BUILD_DIR) -lImGui $(SRC_FILES) -o $(VP)/ImGuizmo/ImGuizmo.dll
	cp $(VP)/ImGuizmo/ImGuizmo.dll $(BUILD_DIR)/ImGuizmo.dll
