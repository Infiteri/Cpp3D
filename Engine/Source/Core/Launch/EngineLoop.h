#pragma once

#include "Base.h"
#include "Core/Application.h"
#include "Core/Engine.h"

namespace Core
{
    class CE_API EngineLoop
    {
    private:
        Application *app;

    public:
        EngineLoop(Application *inApplication);
        ~EngineLoop();

        void Start();
        void Run();
        void End();
    };
} // namespace Core
