#pragma once

#include "Base.h"
#include "Core/Data/CeDataSet.h"
#include "Renderer/Color.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture/CubemapTexture.h"

namespace Core
{
    enum class SkyMode
    {
        Color,
        Skybox,
        Shader
    };

    class CE_ED_API Sky
    {
    private:
        SkyMode mode;

        Color color;

        CubemapTexture *cubemap = nullptr;
        std::string cubemapLoadPath = "";

        Shader *shader = nullptr;
        CeDataSet shaderData;
        std::string shaderName = "";

        void _DestroyFromCurrentMode();

    public:
        Sky();
        ~Sky();

        void SetColorMode(const Color &color);
        void SetSkyboxMode(CubemapConfiguration &config);
        void SetSkyboxMode(const std::string &name);
        void SetShaderMode(const std::string &name);

        void Render();

        void From(Sky &sky);

        inline SkyMode GetMode() { return mode; };
        inline Color &GetColor() { return color; };
        inline CeDataSet *GetShaderDataSet() { return &shaderData; };
        inline std::string GetCubemapLoadPath() { return cubemapLoadPath; };
        inline std::string GetShaderName() { return shaderName; };
    };
} // namespace Core
