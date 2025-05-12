@ECHO OFF

echo Is this a "global" or "local" build? (Type G for global, L for local)
set /p buildType="Enter G or L: "

if /i "%buildType%"=="L" (
    echo Setting up global build...
    set "SRC_DIRS=src src\contrib"
    set "INCLUDE_PATH=include"
    set "OUTPUT_FILE=yaml.dll"
) else if /i "%buildType%"=="G" (
    echo Setting up local build...
    set "SRC_DIRS=Engine/Vendor/yaml/src Engine/Vendor/yaml/src/contrib"
    set "INCLUDE_PATH=Engine/Vendor/yaml/include"
    set "OUTPUT_FILE=Engine/Vendor/yaml/src/yaml.dll"
) else (
    echo Invalid option.
    exit /b
)

setlocal EnableDelayedExpansion
set "CPP_FILES="
for %%D in (%SRC_DIRS%) do (
    for %%F in (%%D\*.cpp) do (
        if exist %%F (
            set "CPP_FILES=!CPP_FILES! %%F"
        )
    )
)

:: Compile command
echo "Building"
g++ -g -shared -I%INCLUDE_PATH% %CPP_FILES% -o %OUTPUT_FILE%

if %ERRORLEVEL%==0 (
    echo Build succeeded! DLL saved to %OUTPUT_FILE%.
) else (
    echo Build failed. Check the error messages above.
)
