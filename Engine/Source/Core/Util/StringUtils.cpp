#include "StringUtils.h"

namespace Core
{
    namespace StringUtils
    {
        std::string GetFilenameExtension(const std::string &name)
        {
            size_t dotPos = name.find_last_of('.');
            std::string ext = (dotPos != std::string::npos) ? name.substr(dotPos + 1) : "";
            return ext;
        }

        std::string GetFilenameFromPath(const std::string &path)
        {
            size_t slash = path.find_last_of("/\\");
            return (slash == std::string::npos) ? path : path.substr(slash + 1);
        }
    } // namespace StringUtils
} // namespace Core
