@ECHO OFF

echo Setting up vendor...

if NOT EXIST "Bin\" (
  pushd ..
)

copy /y "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\glfw3.dll"
copy /y "Engine\Vendor\ImGui\ImGui.dll" "Bin\ImGui.dll"
copy /y "Engine\Vendor\glad\lib\glad.dll" "Bin\glad.dll"

popd 
