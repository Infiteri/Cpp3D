#include "Renderer.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Material/MaterialSystem.h"
#include "Renderer/Object/Mesh.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Shader/ShaderSystem.h"

#include "Camera/PerspectiveCamera.h"
#include "Renderer/Texture/Texture2D.h"
#include "Renderer/Texture/TextureSystem.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Core
{
    static Renderer::State state;
    static Mesh *mesh;
    static Color BG{125, 125, 125, 255};

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

        mesh = new Mesh();
        mesh->GetMaterial()->SetColorTexture("crate.png");

        glEnable(GL_MULTISAMPLE);
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

        shhd->Use();
        shhd->Mat4(camera->GetProjection(), "uProjection");
        shhd->Mat4(camera->GetInvertedView(), "uView");
        mesh->Render(shhd);
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

    Mesh *Renderer::GetMesh() { return mesh; }
} // namespace Core
