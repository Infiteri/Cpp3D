#pragma once

#include "Base.h"
#include "Window/Window.h"

namespace Core
{
    class CE_API Engine
    {
    public:
        struct State
        {
            Window *Window;
        };

    public:
        Engine();
        ~Engine();

        static void InitializeLogger();
        static void InitializeSubsystems();

        static void PreInit();
        static void Init();

        static bool ShouldRun();
        static void Render();
        static void Update();

        static void Shutdown();

        static Window *GetWindow();
    };
} // namespace Core
