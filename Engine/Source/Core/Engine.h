#pragma once

#include "Base.h"
#include "Window/Window.h"
#include <memory>

namespace Core
{
    class CE_API Engine
    {
    public:
        /// @brief Engine internal way of representing project state
        enum class ProjectState
        {
            Default,
            UserLoaded
        };

        struct State
        {
            std::unique_ptr<Window> Window;
            ProjectState ProjState;
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

        static ProjectState GetProjectState();

        /// @brief: Returns a pointer to the active Window.
        /// @note: The Engine is the owner of this object. Do not call 'delete' on this pointer.
        /// @todo: Return a shared pointer to the window so that calling 'delete' is not possible.
        static Window *GetWindow();
    };
} // namespace Core
