@ECHO OFF

echo Is this a "global" or "local" build? (Type G for global, L for local)
set /p buildType="Enter G or L: "

if /i "%buildType%"=="G" (
gcc -g -shared -IEngine/Vendor/glad/include Engine/Vendor/glad/src/glad.c -o Engine/Vendor/glad/lib/glad.dll 
) else if /i "%buildType%"=="L" (
gcc -g -shared -Iinclude src/glad.c -o lib/glad.dll 
) else (
    echo Invalid option. 
    exit /b
)

