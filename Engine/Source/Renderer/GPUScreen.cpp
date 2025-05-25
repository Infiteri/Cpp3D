#include "GPUScreen.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader/ShaderSystem.h"

#include <glad/glad.h>

namespace Core
{
    static float screenQuadVertices[] = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

    void GPUScreen::Create()
    {
        Array = new VertexArray();
        Array->GenerateVertexBuffer(
            screenQuadVertices,
            sizeof(screenQuadVertices)); // ! NO sizeof(float) cuz it makes the final image broken;
        Array->GetVertexBuffer()->AddLayout(0, 0, 2);
        Array->GetVertexBuffer()->AddLayout(1, 2, 2);

        {
            FrameBuffer::Configuration config;
            config.Width = Renderer::GetViewport().Width;
            config.Height = Renderer::GetViewport().Height;
            config.Passes.push_back({FrameBuffer::Rgb});
            config.Passes.push_back({FrameBuffer::Depth});
            Buffer = new FrameBuffer(config);
        }

        {
            FrameBuffer::Configuration config;
            config.Width = Renderer::GetViewport().Width;
            config.Height = Renderer::GetViewport().Height;
            config.Passes.push_back({FrameBuffer::Rgb});
            config.Passes.push_back({FrameBuffer::Depth});
            PostBuffer = new FrameBuffer(config);
        }

        Shader = ShaderSystem::GetEngineResource("Screen.glsl");
    }

    void GPUScreen::Destroy()
    {
        delete Array;
        delete Buffer;
        delete PostBuffer;
    }

    void GPUScreen::Begin()
    {
        Buffer->Bind();
        glEnable(GL_DEPTH_TEST);
    }

    void GPUScreen::End()
    {
        Buffer->Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

} // namespace Core
