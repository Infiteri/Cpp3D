#pragma once

#include "Core/Application.h"
#include "Core/Launch/EngineLoop.h"

extern Core::Application *Core::CreateApplication();

int main()
{
    Core::EngineLoop loop(Core::CreateApplication());
    loop.Start();
    loop.Run();
    loop.End();
}
