#pragma once

#include "Platform/Library.h"
#include "Script/ActorScript.h"
#include <memory>
#include <unordered_map>

namespace Core
{
    class ScriptEngine
    {
    public:
        struct State
        {
            std::unordered_map<UUID, std::unique_ptr<ActorScript>> Scripts;
            Library Library;
        };

    public:
        static void Init();
        static void Shutdown();

        static void LoadLibrary(const std::string &libPath);

        static void OnRuntimeStart();
        static void OnRuntimeStop();
        static void OnRuntimeUpdate();

        static void ClearScriptSet();

        static void RegisterScript(ActorScript *instance, Actor *owner);
        static void RegisterScript(const std::string &className, Actor *owner);
    };
} // namespace Core
