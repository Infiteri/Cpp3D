#include "Platform.h"

#ifdef CE_WIN32

#include <windows.h>

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
} // namespace Core

#endif
