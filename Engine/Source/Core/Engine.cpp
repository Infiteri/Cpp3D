#include "Engine.h"
#include "Core/Data/CeData.h"
#include "Core/Event/EventSystem.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Logger.h"
#include "Core/Window/Window.h"
#include "Layer/ImGuiLayer.h"
#include "Platform/Library.h"
#include "Project/ProjectSystem.h"
#include "Renderer/Renderer.h"
#include "Scene/World.h"
#include "Script/ScriptEngine.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>

namespace Core
{
    static Engine::State state;

    void Engine::InitializeLogger()
    {
        Logger::Init();

        CE_DEFINE_LOG_CATEGORY("CE_RENDER", "Render");
        CE_DEFINE_LOG_CATEGORY("CE_SCENE", "Scene");
    }

    void Engine::InitializeSubsystems()
    {
        LayerStack::Init();
        ImGuiLayer::Init();
        EventSystem::Init();
        Renderer::Init();
        ScriptEngine::Init();
        ProjectSystem::Init();

        World::Init();
    }

    void Engine::PreInit()
    {
        InitializeLogger();
        CE_INFO("Initializing Engine...");

        Window::Configuration config;
        config.Size = Window::Windowed;
        state.Window = std::make_unique<Window>(config);

        InitializeSubsystems();
    }

    void Engine::Init()
    {
        // todo: Some kind of project initialization code here? maybe runtime
        // note: Post project loading here
        {
            auto project = ProjectSystem::GetActiveProject();

            if (!project)
            {
                CE_WARN("No active project specified, creating empty project");
                ProjectSystem::New();
                project = ProjectSystem::GetActiveProject();
            }

            if (!project->GetLibraryPath().empty())
                ScriptEngine::LoadLibrary(project->GetLibraryPath());
        }
    }

    bool Engine::ShouldRun() { return state.Window->ShouldRun(); }

    void Engine::Render()
    {
        Renderer::BeginFrame();
        Renderer::Render();
        World::RenderActive();
        Renderer::EndFrame();

        ImGuiLayer::BeginFrame();
        LayerStack::RenderImGui();
        ImGuiLayer::EndFrame();
    }

    void Engine::Update()
    {
        EventSystem::Update();
        LayerStack::Update();
        // note: Not wanted right now
        // todo:       World::UpdateActive();
        state.Window->Update();
    }

    void Engine::Shutdown()
    {
        ImGuiLayer::Shutdown();
        LayerStack::Shutdown();
        EventSystem::Shutdown();
        ScriptEngine::Shutdown();
        ProjectSystem::Shutdown();

        World::Shutdown();
    }

    Window *Engine::GetWindow() { return state.Window.get(); }

    Engine::ProjectState Engine::GetProjectState() { return state.ProjState; }
} // namespace Core
