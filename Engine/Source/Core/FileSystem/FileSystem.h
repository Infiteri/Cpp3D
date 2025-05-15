#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class CE_API FileSystem
    {
    public:
        FileSystem() {};
        ~FileSystem() {};

        static std::string ReadFile(const std::string &name);
        static void WriteFile(const std::string &name, const std::string &content);
        static void CreateFile(const std::string &name);
        static bool ExistsFile(const std::string &name);
        static void RemoveFile(const std::string &name);
        static void CopyFile(const std::string &source, const std::string &dest);
    };
} // namespace Core
