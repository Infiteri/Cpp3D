@ECHO OFF

echo Is this a "global" or "local" build? (Type G for global, L for local)
set /p buildType="Enter G or L: "

if /i "%buildType%"=="G" (
    echo Setting up global build...
    set "cppFiles=Engine/Vendor/ImGui/imgui.cpp Engine/Vendor/ImGui/imgui_demo.cpp Engine/Vendor/ImGui/imgui_draw.cpp Engine/Vendor/ImGui/imgui_tables.cpp Engine/Vendor/ImGui/imgui_widgets.cpp Engine/Vendor/ImGui/backends/imgui_impl_glfw.cpp Engine/Vendor/ImGui/backends/imgui_impl_opengl3.cpp"
    set "INCLUDE_PATH=Engine/Vendor/ImGui -IEngine/Vendor/GLFW/include"
    set "LIBRARY_PATH=Bin"
    set "OUTPUT_FILE=Engine/Vendor/ImGui/ImGui.dll"
) else if /i "%buildType%"=="L" (
    echo Setting up local build...
    set "cppFiles=imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp"
    set "INCLUDE_PATH=. -I../GLFW/include"
    set "LIBRARY_PATH=../../../Bin"
    set "OUTPUT_FILE=ImGui.dll"
) else (
    echo Invalid option.
    exit /b
)

:: Compile command
echo Compiling DLL...
g++ -g -shared -I%INCLUDE_PATH% -L%LIBRARY_PATH% -lglfw3 %cppFiles% -o %OUTPUT_FILE%

if %ERRORLEVEL%==0 (
    echo Build succeeded! DLL saved to %OUTPUT_FILE%.
) else (
    echo Build failed. Check the error messages above.
)
