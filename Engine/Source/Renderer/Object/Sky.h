#pragma once

#include "Base.h"
#include "Renderer/Color.h"
#include "Renderer/Texture/CubemapTexture.h"

namespace Core
{
    enum class SkyMode
    {
        Color,
        Skybox
    };

    class CE_ED_API Sky
    {
    private:
        SkyMode mode;
        Color color;
        CubemapTexture *cubemap = nullptr;
        std::string cubemapLoadPath = "";

        void _DestroyFromCurrentMode();

    public:
        Sky();
        ~Sky();

        void SetColorMode(const Color &color);
        void SetSkyboxMode(CubemapConfiguration &config);
        void SetSkyboxMode(const std::string &name);

        void Render();

        inline SkyMode GetMode() { return mode; };
        inline Color &GetColor() { return color; };
        inline std::string GetCubemapLoadPath() { return cubemapLoadPath; };
    };
} // namespace Core
