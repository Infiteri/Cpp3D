#pragma once

#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture/CubemapTexture.h"

namespace Core
{
    class Skybox
    {
    private:
        VertexArray *array;
        Shader *shader;

        friend class Renderer;

    public:
        Skybox();
        ~Skybox();

        void Init();

        void RenderCubemap(CubemapTexture *cubemap);
    };
} // namespace Core
