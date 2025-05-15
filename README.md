# CORE ENGINE

Small and simple engine built for fun as a side project.

## BUILD

Its built with "make" and MSYS2 UCRT64 Toolchain (not that it matters as long as g++ is present)
Requirements:
    - C++ Compiler (g++)
    - Make
    - win32

With these components in place run "Build.bat" in the root directory. If there are any errors when building that come from 'make' and not 'g++' make sure to run the build command from a linux based console (MSYS2 or even GIT bash, enter one of these consoles, run 'cd PROJECT_PATH' (where PROJECT_PATH is the path of the Engine folder) and run 'start', that should open 'cmd' in the Engine root directory, from there call 'Build.bat') 

## VENDOR
Used libraries:
    GLAD
	ImGui
    GLFW
