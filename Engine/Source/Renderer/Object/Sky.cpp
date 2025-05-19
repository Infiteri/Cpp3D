#include "Sky.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture/CubemapTexture.h"
#include "Resource/CubemapLoader.h"

#include <glad/glad.h>

namespace Core
{
    Sky::Sky() { cubemap = nullptr; }

    Sky::~Sky() { _DestroyFromCurrentMode(); }

    void Sky::_DestroyFromCurrentMode()
    {
        switch (mode)
        {
        case SkyMode::Skybox:
            if (cubemap)
                delete cubemap;
            cubemap = nullptr;
            break;

        default:
            break;
        }
    }

    void Sky::SetColorMode(const Color &color)
    {
        _DestroyFromCurrentMode();
        mode = SkyMode::Color;

        this->color = color;
    }

    void Sky::SetSkyboxMode(CubemapConfiguration &color)
    {
        _DestroyFromCurrentMode();
        mode = SkyMode::Skybox;

        cubemap = new CubemapTexture();
        cubemap->Load(color);
    }

    void Sky::SetSkyboxMode(const std::string &name)
    {
        CubemapLoader loader;
        CubemapConfiguration config;
        loader.Deserialize(config, name);
        SetSkyboxMode(config);
    }

    void Sky::Render()
    {
        switch (mode)
        {
        case SkyMode::Color:
            glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;

        case SkyMode::Skybox:
            // todo:
            if (cubemap)
                Renderer::RenderCubemapTexture(cubemap);
            break;
        }
    }
} // namespace Core
