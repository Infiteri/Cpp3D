#pragma once

#include "Core/Application.h"
#include "Core/Launch/EngineLoop.h"

extern Core::Application *Core::CreateApplication(ApplicationArguments &arguments);

int main(int argc, const char **argv)
{
    Core::ApplicationArguments args;
    args.Count = argc;

    for (int i = 1; i < argc; i++)
        args.Arguments.push_back(argv[i]);

    Core::EngineLoop loop(Core::CreateApplication(args));
    loop.Start();
    loop.Run();
    loop.End();
}
