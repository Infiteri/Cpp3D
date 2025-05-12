#include "Platform.h"

#ifdef CE_WIN32

#include "Core/Engine.h"

#include <windows.h>

// window handle specfici needed for file dialogs
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Core
{
    void Platform::SetConsoleColor(Platform::ConsoleColor color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD colorAttribute = FOREGROUND_INTENSITY; // Default to high-intensity (bright) text

        switch (color)
        {
        case ConsoleColor::White:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Gray:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Red:
            colorAttribute |= FOREGROUND_RED;
            break;

        case ConsoleColor::DarkRed:
            colorAttribute |= FOREGROUND_RED;
            break;

        case ConsoleColor::Blue:
            colorAttribute |= FOREGROUND_BLUE;
            break;

        case ConsoleColor::Green:
            colorAttribute |= FOREGROUND_GREEN;
            break;

        case ConsoleColor::Yellow:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN;
            break;

        default:
            // Default to white if an unsupported color is specified
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }

        SetConsoleTextAttribute(hConsole, colorAttribute);
    }

    void Platform::LogMessage(const char *msg)
    {
        OutputDebugStringA(msg);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD bytesWritten;
        WriteConsoleA(hConsole, msg, strlen(msg), &bytesWritten, NULL);
    }

    std::string Platform::OpenFileDialog(const char *filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(Engine::GetWindow()->GetHandle());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;

        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return "";
    }

    std::string Platform::SaveFileDialog(const char *filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Engine::GetWindow()->GetHandle());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return "";
    }

} // namespace Core

#endif
