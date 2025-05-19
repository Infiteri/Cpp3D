#pragma once

#include "Base.h"
#include "Texture.h"
#include <string>

namespace Core
{
    struct CubemapConfiguration
    {
        std::string Left;
        std::string Right;
        std::string Top;
        std::string Bottom;
        std::string Front;
        std::string Back;
    };

    class CubemapTexture : public Texture
    {
    private:
        CubemapConfiguration state;

        void _EnsureDestructionID();
        void Load(u32 side, int w, int h, int channel, u8 *data);

    public:
        CubemapTexture();
        ~CubemapTexture();

        void Load();
        void Bind() const;
        void Unbind() const;
        void Destroy();

        void Use();

        void Load(CubemapConfiguration &config);
    };
} // namespace Core
