#include "Renderer.h"
#include "Camera/PerspectiveCamera.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/Light.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Material/MaterialSystem.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Renderer/Texture/TextureSystem.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Core
{
    static Renderer::State state;
    static Color BG{125, 125, 125, 255};
    static DirectionalLight light;

    // easier to keep track of what gets started and gets shutdown
    static void InitializeRendererSubsystems()
    {
        CameraSystem::Init();
        ShaderSystem::Init();
        TextureSystem::Init();
        MaterialSystem::Init();
    }

    static void ShutdownRendererSubsystems()
    {
        CameraSystem::Shutdown();
        ShaderSystem::Shutdown();
        TextureSystem::Shutdown();
        MaterialSystem::Shutdown();
    }

    void Renderer::SetColor(const Color &color)
    {
        BG.r = color.r;
        BG.g = color.g;
        BG.b = color.b;
    }

    Color &Renderer::GetColor() { return BG; }

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

        state.Screen.Create();

        CameraSystem::AddPerspectiveCamera("Renderer",
                                           {90, state.ScreenViewport.GetAspect(), 0.01f, 1000.0f});
        CameraSystem::ActivatePerspectiveCamera("Renderer");
        CameraSystem::GetActivePerspective()->SetPosition({0, 0, 5});

        glEnable(GL_MULTISAMPLE);

        light.Direction = {-1, 0, -1};
    }

    void Renderer::Shutdown()
    {
        CE_VERIFY(state.HasContext);

        ShutdownRendererSubsystems();
        state.Screen.Destroy();
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
    }

    ViewportState &Renderer::GetViewport() { return state.ScreenViewport; }

    void Renderer::BeginFrame()
    {
        CE_VERIFY(state.HasContext);

        LightIDManager::BeginFrame();

        state.Screen.Begin();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::Render()
    {
        CE_VERIFY(state.HasContext);

        glClearColor(BG.r / 255, BG.g / 255, BG.b / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto shhd = ShaderSystem::GetEngineResource("Object.glsl");
        auto camera = CameraSystem::GetActivePerspective();
        light.Render(shhd);

        shhd->Use();
        UploadCameraToShader(shhd, camera);

        shhd->Int(LightIDManager::PointGetLastFrameCount(), "uPointLightCount");
        shhd->Int(LightIDManager::SpotGetLastFrameCount(), "uSpotLightCount");
    }

    void Renderer::EndFrame()
    {
        CE_VERIFY(state.HasContext);

        state.Screen.End();

        auto pass = state.Screen.Buffer->GetRenderPass(0);
        state.Screen.Array->Bind();
        state.Screen.Shader->Use();

        glActiveTexture(GL_TEXTURE0 + pass->Index);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.Shader->Int(pass->Index, "uScreenTexture");

        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();
    }

    void Renderer::UploadCameraToShader(Shader *shader, PerspectiveCamera *camera)
    {
        CE_VERIFY(shader && camera);

        shader->Use();
        shader->Mat4(camera->GetProjection(), "uProjection");
        shader->Mat4(camera->GetInvertedView(), "uView");
        shader->Vec3(camera->GetPosition(), "uCameraPosition");
    }

    u32 Renderer::GetSceneViewportPassID() { return state.Screen.Buffer->GetRenderPass(0)->Id; }

} // namespace Core
