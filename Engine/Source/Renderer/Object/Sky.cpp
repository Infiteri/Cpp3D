#include "Sky.h"
#include "Core/Data/CeData.h"
#include "Math/Vector.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture/CubemapTexture.h"
#include "Resource/CubemapLoader.h"

#include "Core/Logger.h"

#include <glad/glad.h>

namespace Core
{
    Sky::Sky()
    {
        cubemap = nullptr;
        mode = SkyMode::Color;

        shaderData.Add("uTint", CeDataType::Vector2, new Vector2(1, 1));
    }

    Sky::~Sky() { _DestroyFromCurrentMode(); }

    void Sky::_DestroyFromCurrentMode()
    {
        switch (mode)
        {
        case SkyMode::Skybox:
            if (cubemap)
                delete cubemap;
            cubemap = nullptr;
            cubemapLoadPath = "";
            break;

        case SkyMode::Shader:
            if (shader)
                delete shader;
            shader = nullptr;
            shaderName = "";
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
        cubemapLoadPath = name;
    }

    void Sky::SetShaderMode(const std::string &name)
    {
        _DestroyFromCurrentMode();
        mode = SkyMode::Shader;

        shader = new Shader(name);
        shaderName = name;
    }

    void Sky::Render()
    {
        switch (mode)
        {
        case SkyMode::Color:
            glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;

        case SkyMode::Shader:
            if (shader)
            {
                shader->Use();
                for (auto &[name, data] : shaderData.GetDataSet())
                {
                    const char *cName = data->GetName().c_str();
                    switch (data->GetType())
                    {
                    case CeDataType::Vector2:
                    {
                        Vector2 *c = data->As<Vector2>();
                        shader->Vec2({c->x, c->y}, cName);
                    }
                    break;
                    case CeDataType::Vector3:
                    {
                        Vector3 *c = data->As<Vector3>();
                        shader->Vec3({c->x, c->y, c->z}, cName);
                    }
                    break;
                    case CeDataType::Vector4:
                    {
                        Vector4 *c = data->As<Vector4>();
                        shader->Vec4({c->x, c->y, c->z, c->w}, cName);
                    }
                    break;
                    case CeDataType::Color:
                    {
                        Color *c = data->As<Color>();
                        shader->Vec4({c->r, c->g, c->b, c->a}, cName);
                    }
                    break;
                    case CeDataType::Float:
                    {
                        float *c = data->As<float>();
                        shader->Float(*c, cName);
                    }
                    break;
                    }
                }

                Renderer::RenderCubemapShader(shader);
            }
            break;

        case SkyMode::Skybox:
            if (cubemap)
                Renderer::RenderCubemapTexture(cubemap);
            break;
        }
    }
} // namespace Core
