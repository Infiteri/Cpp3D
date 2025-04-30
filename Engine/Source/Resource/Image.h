#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class CE_API Image
    {
    private:
        std::string name = "";
        int width = 0;
        int height = 0;
        int channels = 0;
        u8 *data = 0;
        bool isValid = false;

    public:
        Image() = default;
        Image(const std::string &name);
        ~Image();

        void Load(const std::string &name);
        void DestroyData();

        inline std::string GetName() { return name; };
        inline int GetWidth() { return width; };
        inline int GetHeight() { return height; };
        inline int GetChannels() { return channels; };
        inline u8 *GetData() { return data; };
        inline bool IsValid() { return isValid; };
    };
} // namespace Core
