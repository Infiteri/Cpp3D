#pragma once

#include "Resource/Image.h"
#include "Texture.h"
#include <string>

namespace Core
{
    class CE_API Texture2D : public Texture
    {
    private:
        Image *imageInstance;

        void EnsureDestructionID();
        void DestroyImage();

    public:
        Texture2D();
        ~Texture2D();

        void Load();
        void Load(int width, int height, int channels, u8 *data);
        void Load(const std::string &name);
        void Bind() const;
        void Unbind() const;
        void Destroy();

        inline std::string GetImagePath()
        {
            if (!imageInstance)
                return "";
            return imageInstance->GetName();
        };

        void Use() const;
    };
} // namespace Core
