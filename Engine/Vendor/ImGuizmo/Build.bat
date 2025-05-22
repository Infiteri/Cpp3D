@echo off
setlocal

:: Choose build type
set /p BUILD_TYPE=Build Type [L for Local / G for Global]: 

:: Compiler & flags
set CXX=g++
set CXXFLAGS=-I../ImGui -std=c++17 -O2 -Wall

:: Object files
set OBJS=GraphEditor.o ImCurveEdit.o ImGradient.o ImGuizmo.o ImSequencer.o

:: Set include & lib dirs based on build type
if /I "%BUILD_TYPE%"=="L" (
    echo Using Local paths...
    set INCLUDES=-I.
    set LIBDIR=-L../../../Bin
    set LINK_LIBS=-lImGui
) else if /I "%BUILD_TYPE%"=="G" (
    echo Using Global paths...
    set INCLUDES=-I.
    set LIBDIR=-LC:\Dev\Libs\Bin
    set LINK_LIBS=-lImGui
) else (
    echo Invalid build type! Use L or G.
    exit /b 1
)

 Compile (if needed, uncomment to compile)
 for %%f in (*.cpp) do (
     echo Compiling %%f ...
     %CXX% %CXXFLAGS% -c %%f -o %%~nf.o
 )

:: Link DLL
echo Linking into ImGuizmo.dll...
%CXX% -shared -g %INCLUDES% %OBJS% -o ImGuizmo.dll %LIBDIR% %LINK_LIBS%

echo Build complete.

endlocal
