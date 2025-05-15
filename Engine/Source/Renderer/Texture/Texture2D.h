#pragma once

#include "Resource/Image.h"
#include "Texture.h"
#include <string>

namespace Core
{
    class CE_API Texture2D : public Texture
    {
    private:
        std::string imageLoadPath = "";
        void EnsureDestructionID();

    public:
        Texture2D();
        ~Texture2D();

        void Load();
        void Load(int width, int height, int channels, u8 *data);
        void Load(const std::string &name);
        void Bind() const;
        void Unbind() const;
        void Destroy();

        inline std::string GetImagePath() { return imageLoadPath; };

        void Use() const;
    };
} // namespace Core
