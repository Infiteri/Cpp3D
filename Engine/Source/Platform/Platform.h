#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class Platform
    {
    public:
        enum ConsoleColor
        {
            White,
            Red,
            DarkRed,
            Blue,
            Gray,
            Green,
            Yellow,
        };

    public:
        Platform() {};
        ~Platform() {};

        // -- Console related --
        static void SetConsoleColor(ConsoleColor color);
        static void LogMessage(const char *message);
        // ---------------------

        // -- File Dialogs --
        static CE_ED_API std::string OpenFileDialog(const char *filter);
        static CE_ED_API std::string SaveFileDialog(const char *filter);
        // ------------------
    };
} // namespace Core
