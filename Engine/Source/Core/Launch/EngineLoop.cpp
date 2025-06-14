#include "EngineLoop.h"
#include "Core/Engine.h"
#include <cinttypes>

namespace Core
{
    EngineLoop::EngineLoop(Application *inApplication) { app = inApplication; }

    EngineLoop::~EngineLoop() { delete app; }

    void EngineLoop::Start()
    {
        Engine::PreInit();
        app->Init();
        Engine::Init();
    }

    void EngineLoop::Run()
    {
        while (Engine::ShouldRun())
        {
            Engine::Update();
            app->Update();
            Engine::Render();
        }
    }

    void EngineLoop::End()
    {
        app->Destroy();
        Engine::Shutdown();
        delete app;
    }

} // namespace Core
