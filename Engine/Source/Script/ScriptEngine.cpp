#include "ScriptEngine.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Script/ActorScript.h"
#include <memory>

namespace Core
{
    static ScriptEngine::State state;

    typedef ActorScript *(*GetScriptPFN)();

    static bool Exists(const UUID &name) { return state.Scripts.find(name) != state.Scripts.end(); }

    void ScriptEngine::Init() { CE_DEFINE_LOG_CATEGORY("CE_SCRIPT", "Script"); }

    void ScriptEngine::Shutdown() { ClearScriptSet(); }

    void ScriptEngine::LoadLibrary(const std::string &libPath)
    {
        if (state.Library.IsValid())
            state.Library.Destroy();

        state.Library.Create(libPath);
    }

    void ScriptEngine::OnRuntimeStart()
    {
        for (auto &script : state.Scripts)
            script.second->OnStart();
    }

    void ScriptEngine::OnRuntimeStop()
    {
        for (auto &script : state.Scripts)
            script.second->OnStart();
    }

    void ScriptEngine::OnRuntimeUpdate()
    {
        for (auto &script : state.Scripts)
            script.second->OnUpdate();
    }

    void ScriptEngine::ClearScriptSet() { state.Scripts.clear(); }

    void ScriptEngine::RegisterScript(ActorScript *instance, Actor *owner)
    {
        CE_VERIFY(instance && owner);

        if (Exists(owner->GetID()))
            return;

        state.Scripts[owner->GetID()] = std::unique_ptr<ActorScript>(instance);
    }

    void ScriptEngine::RegisterScript(const std::string &className, Actor *owner)
    {
        if (className.empty() || !owner)
        {
            CE_LOG("CE_SCRIPT", Error,
                   "Unable to load script instance, no actor or empty class name");
            return;
        }

        if (!state.Library.IsValid())
        {
            CE_LOG("CE_SCRIPT", Error, "Unable to create script, invalid script library.");
            return;
        }

        auto pfn = state.Library.GetFunction<GetScriptPFN>("CE_CREATE_SCRIPT_PFN_" + className);
        if (!pfn)
        {
            CE_LOG("CE_SCRIPT", Error, "Unable to load PFN, unable to create script");
            return;
        }

        auto instance = pfn();
        if (!instance)
        {
            CE_LOG("CE_SCRIPT", Error, "Actor Script class instance is null");
            return;
        }

        RegisterScript(instance, owner);
    }
} // namespace Core
