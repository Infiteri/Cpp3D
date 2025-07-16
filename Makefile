export BUILD_DIR := Bin
export OBJ_DIR := Bin-Obj
export DEFINES := -D_DEBUG -DCE_WITH_EDITOR -DCE_WIN32
export VENDOR_INCLUDE := -IEngine/Vendor/GLFW/include -IEngine/Vendor/glad/include -IEngine/Vendor/ImGui -IEngine/Vendor/stb -IEngine/Vendor/YAML/include -IEngine/Vendor/ImGuizmo
export COMPILER_FLAGS := -g -std=c++17

# todo: Better commands ? Like PascalCase 
.PHONY: all engine editor vendor-copy clean-obj

all: engine editor

engine:
	@$(MAKE) -f Engine/Makefile all

editor:
	@$(MAKE) -f Editor/Makefile all


clean-obj:
	rm -rf $(OBJ_DIR)

vendor-copy:
	cp "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\glfw3.dll"
	cp "Engine\Vendor\ImGui\ImGui.dll" "Bin\ImGui.dll"
	cp "Engine\Vendor\ImGuizmo\ImGuizmo.dll" "Bin\ImGuizmo.dll"
	cp "Engine\Vendor\YAML\YAML.dll" "Bin\YAML.dll"
	cp "Engine\Vendor\glad\lib\glad.dll" "Bin\glad.dll"
