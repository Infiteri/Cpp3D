#pragma once

#include "Base.h"
#include <string>
#include <vector>

namespace Core
{
    struct DirectoryEntry
    {
        std::string Name;
        bool IsFolder = false;
    };

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

        // -- Directory Entry  --
        static CE_ED_API std::vector<std::string>
        GetFilePathsInDirectory(const std::string &directoryPath);
        static CE_ED_API std::vector<std::string>
        GetFolderPathsInDirectory(const std::string &directoryPath);
        static CE_ED_API std::vector<DirectoryEntry>
        GetDirectoryEntries(const std::string &directoryPath);
        // ----------------------
    };
} // namespace Core
