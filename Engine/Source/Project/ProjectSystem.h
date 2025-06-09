#pragma once

#include "Base.h"
#include "Project/Project.h"

namespace Core
{
    class CE_API ProjectSystem
    {
    public:
        struct State
        {
            Project *ActiveProject;
        };

    public:
        static void Init();
        static void Shutdown();

        static Project *GetActiveProject();

        static void New();
        static void Load(const std::string &name);
    };
} // namespace Core
