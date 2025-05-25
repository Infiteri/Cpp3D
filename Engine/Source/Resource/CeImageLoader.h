#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class CE_API CeImageLoader
    {
    public:
        CeImageLoader() = default;
        ~CeImageLoader() = default;

        void CreateCeImage(const std::string &input, const std::string &output);
        char *LoadCeImage(const std::string &input, int *width, int *height, int *channels);
    };
} // namespace Core
