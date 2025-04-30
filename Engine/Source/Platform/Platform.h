#pragma once

#include "Base.h"

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
    };
} // namespace Core
