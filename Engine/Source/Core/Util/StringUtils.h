#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    namespace StringUtils
    {
        CE_API std::string GetFilenameExtension(const std::string &name);
        CE_API std::string GetFilenameFromPath(const std::string &path);
    } // namespace StringUtils
} // namespace Core
