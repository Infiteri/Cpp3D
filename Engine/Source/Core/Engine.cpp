#include "Engine.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Logger.h"
#include "Core/Window/Window.h"
#include "Layer/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Core
{
    static Engine::State state;

    void Engine::InitializeLogger()
    {
        Logger::Init();

        CE_DEFINE_LOG_CATEGORY("CE_RENDER", "Render");
    }

    void Engine::InitializeSubsystems()
    {
        LayerStack::Init();
        ImGuiLayer::Init();

        Renderer::Init();
    }

    void Engine::PreInit()
    {
        InitializeLogger();
        CE_INFO("Initializing Engine...");

        Window::Configuration config;
        config.Size = Window::Windowed;
        state.Window = new Window(config);

        InitializeSubsystems();
    }

    void Engine::Init() {}

    bool Engine::ShouldRun() { return state.Window->ShouldRun(); }

    void Engine::Render()
    {
        Renderer::BeginFrame();
        Renderer::Render();
        Renderer::EndFrame();

        ImGuiLayer::BeginFrame();
        LayerStack::OnImGuiRender();
        ImGuiLayer::EndFrame();
    }

    void Engine::Update()
    {
        LayerStack::OnUpdate();
        state.Window->Update();
    }

    void Engine::Shutdown()
    {
        ImGuiLayer::Shutdown();
        LayerStack::Shutdown();
        delete state.Window;
    }

    Window *Engine::GetWindow() { return state.Window; }

} // namespace Core
