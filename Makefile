export BUILD_DIR := Bin
export OBJ_DIR := Bin-Obj
export DEFINES := -D_DEBUG -DCE_WITH_EDITOR -DCE_WIN32
export VENDOR_INCLUDE := -IEngine/Vendor/GLFW/include -IEngine/Vendor/glad/include -IEngine/Vendor/ImGui -IEngine/Vendor/stb -IEngine/Vendor/YAML/include -IEngine/Vendor/ImGuizmo -IEngine/Vendor/bullet/src
export COMPILER_FLAGS := -g -std=c++17

.PHONY: Scratch Full All Engine Editor VendorCopy VendorBuild AssetsCopy CopyCoreFiles Scaffold CleanObj CleanAll EditorManualLink

All: Engine Editor

Scratch: Scaffold VendorBuild Full

Full: Scaffold VendorCopy AssetsCopy CopyCoreFiles Engine Editor

EditorManualLink:
	@$(MAKE) -f Editor/Makefile manual_link

Engine:
	@$(MAKE) -f Engine/Makefile all

Editor:
	@$(MAKE) -f Editor/Makefile all

CleanAll:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)

CleanObj:
	rm -rf $(OBJ_DIR)

VendorCopy:
	cp "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\glfw3.dll"
	cp "Engine\Vendor\ImGui\ImGui.dll" "Bin\ImGui.dll"
	cp "Engine\Vendor\ImGuizmo\ImGuizmo.dll" "Bin\ImGuizmo.dll"
	cp "Engine\Vendor\YAML\YAML.dll" "Bin\YAML.dll"
	cp "Engine\Vendor\glad\lib\glad.dll" "Bin\glad.dll"
	cp "Engine\Vendor\bullet\Bullet.dll" "Bin\Bullet.dll"

VendorBuild:
	cp "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\glfw3.dll"
	$(MAKE) -f Engine\Vendor\ImGui\Makefile All
	$(MAKE) -f Engine\Vendor\ImGuizmo\Makefile All
	$(MAKE) -f Engine\Vendor\YAML\Makefile All
	$(MAKE) -f Engine\Vendor\glad\Makefile All
	$(MAKE) -f Engine\Vendor\bullet\Makefile All

AssetsCopy:
	cp -r "Engine\Assets" "Bin\EngineAssets"
	cp -r "UsedAssets" "Bin\Assets"

CopyCoreFiles:
	cp "Engine\Assets\Proj.ce_proj" "Bin\Proj.ce_proj"
	cp "Engine\Assets\imgui.ini" "Bin\imgui.ini"
	cp "Editor\EditorSettings.ce_settings" "Bin\EditorSettings.ce_settings"
	
Scaffold:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)
