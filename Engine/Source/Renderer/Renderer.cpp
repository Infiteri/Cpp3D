#include "Renderer.h"
#include "Buffer/Buffer.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "Core/Engine.h"
#include "Light/DirectionalLight.h"
#include "Light/Light.h"
#include "Material/MaterialSystem.h"
#include "Object/Sky.h"
#include "Renderer/Object/Skybox.h"
#include "Shader/Shader.h"
#include "Shader/ShaderSystem.h"
#include "Texture/CubemapTexture.h"
#include "Texture/TextureSystem.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Core
{
    static Renderer::State state;

    // easier to keep track of what gets started and gets shutdown
    static void InitializeRendererSubsystems()
    {
        CameraSystem::Init();
        ShaderSystem::Init();
        TextureSystem::Init();
        MaterialSystem::Init();

        state.Skybox.Init();
    }

    static void ShutdownRendererSubsystems()
    {
        CameraSystem::Shutdown();
        ShaderSystem::Shutdown();
        TextureSystem::Shutdown();
        MaterialSystem::Shutdown();
    }

    void Renderer::InitializeGLAD()
    {
        if (state.HasContext)
            return;
        state.HasContext = true;
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void Renderer::Init()
    {
        if (!state.HasContext)
            InitializeGLAD();

        Viewport(Engine::GetWindow()->GetWidth(), Engine::GetWindow()->GetHeight());

        InitializeRendererSubsystems();

        state.Post.Add("EngineAssets/Shaders/Post.glsl", false);

        state.Screen.Create();

        CameraSystem::AddPerspectiveCamera("Renderer",
                                           {90, state.ScreenViewport.GetAspect(), 0.01f, 1000.0f});
        CameraSystem::ActivatePerspectiveCamera("Renderer");
        CameraSystem::GetActivePerspective()->SetPosition({0, 0, 5});

        glEnable(GL_MULTISAMPLE);

        CubemapConfiguration config;
        config.Right = "Assets/Lycksele3/posx.jpg";
        config.Left = "Assets/Lycksele3/negx.jpg";
        config.Top = "Assets/Lycksele3/posy.jpg";
        config.Bottom = "Assets/Lycksele3/negy.jpg";
        config.Front = "Assets/Lycksele3/posz.jpg";
        config.Back = "Assets/Lycksele3/negz.jpg";
        // sky.SetSkyboxMode(config);
    }

    void Renderer::Shutdown()
    {
        CE_VERIFY(state.HasContext);

        ShutdownRendererSubsystems();
        state.Screen.Destroy();
    }

    void Renderer::SetSkyInstance(Sky *sky) { state.SkyInstance = sky; }

    void Renderer::SetDirectioanlLightInstance(DirectionalLight *light)
    {
        state.DirectionalLightInstance = light;
    }

    void Renderer::Viewport(int width, int height)
    {
        CE_VERIFY(state.HasContext);

        state.ScreenViewport.Width = width;
        state.ScreenViewport.Height = height;

        glViewport(0, 0, width, height);

        if (CameraSystem::GetActivePerspective() != nullptr)
            CameraSystem::GetActivePerspective()->UpdateProjection(
                state.ScreenViewport.GetAspect());

        if (state.Screen.Buffer != nullptr)
            state.Screen.Buffer->Resize(width, height);

        if (state.Screen.PostBuffer != nullptr)
            state.Screen.PostBuffer->Resize(width, height);
    }

    ViewportState &Renderer::GetViewport() { return state.ScreenViewport; }

    void Renderer::BeginFrame()
    {
        CE_VERIFY(state.HasContext);

        LightIDManager::BeginFrame();

        state.Screen.Begin();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (state.SkyInstance)
            state.SkyInstance->Render();
    }

    void Renderer::Render()
    {
        CE_VERIFY(state.HasContext);
        auto shhd = ShaderSystem::GetEngineResource("Object.glsl");
        auto camera = CameraSystem::GetActivePerspective();

        shhd->Use();
        UploadCameraToShader(shhd, camera);

        shhd->Int(LightIDManager::PointGetLastFrameCount(), "uPointLightCount");
        shhd->Int(LightIDManager::SpotGetLastFrameCount(), "uSpotLightCount");

        if (state.DirectionalLightInstance)
            state.DirectionalLightInstance->Render(shhd);
    }

    void Renderer::EndFrame()
    {
        CE_VERIFY(state.HasContext);

        state.Screen.End();

        auto pass = state.Screen.Buffer->GetRenderPass(0);
        auto post = state.Screen.PostBuffer;

        post->Bind();
        state.Screen.Shader->Use();

        glActiveTexture(GL_TEXTURE0 + pass->Index);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.Shader->Int(pass->Index, "uScreenTexture");

        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();

        // post processing goes here
        for (auto shader : state.Post.GetEnabledShaders())
        {
            if (!shader)
                continue;

            shader->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->Id);
            state.Screen.Array->Bind();
            state.Screen.Array->GetVertexBuffer()->Bind();
            state.Screen.Array->GetVertexBuffer()->Draw();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->Id);

        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();
    }

    void Renderer::RenderCubemapTexture(CubemapTexture *texture)
    {
        state.Skybox.RenderCubemap(texture);
    }

    void Renderer::RenderCubemapTexture(CubemapTexture *texture, VertexArray *array, Shader *shader,
                                        PerspectiveCamera *camera)
    {
        CE_VERIFY(texture && camera && shader && array);

        glDepthMask(false);
        array->Bind();
        shader->Use();
        UploadCameraToShader(shader, camera);
        shader->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");
        texture->Use();
        shader->Int(0, "uSkybox");
        array->GetVertexBuffer()->Bind();
        array->GetVertexBuffer()->Draw();
        glDepthMask(true);
    }

    void Renderer::UploadCameraToShader(Shader *shader, PerspectiveCamera *camera)
    {
        CE_VERIFY(shader && camera);

        shader->Use();
        shader->Mat4(camera->GetProjection(), "uProjection");
        shader->Mat4(camera->GetInvertedView(), "uView");
        shader->Vec3(camera->GetPosition(), "uCameraPosition");
    }

    u32 Renderer::GetSceneViewportPassID() { return state.Screen.PostBuffer->GetRenderPass(0)->Id; }

} // namespace Core
