#include "ScriptEngine.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Script/ActorScript.h"
#include <memory>

#define CE_DEBUG_SCRIPT_SYS 0

#if CE_DEBUG_SCRIPT_SYS == 1
#define CE_SCR_DBG(msg, ...) CE_LOG("CE_SCRIPT", Trace, msg, ##__VA_ARGS__)
#else
#define CE_SCR_DBG(msg, ...)
#endif

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
        CE_SCR_DBG("Starting script count = %i", state.Scripts.size());
        for (auto &script : state.Scripts)
            script.second->OnStart();
    }

    void ScriptEngine::OnRuntimeStop()
    {
        CE_SCR_DBG("Stoping script count = %i", state.Scripts.size());
        for (auto &script : state.Scripts)
            script.second->OnStop();
    }

    void ScriptEngine::OnRuntimeUpdate()
    {
        for (auto &script : state.Scripts)
        {

            if (script.second->Owner)
                script.second->OnUpdate();
            else
                CE_LOG("CE_SCRIPT", Error, "Unable to update Actor script as the Owner is null");
        }
    }

    void ScriptEngine::ClearScriptSet() { state.Scripts.clear(); }

    void ScriptEngine::RegisterScript(ActorScript *instance, Actor *owner)
    {
        CE_VERIFY(instance && owner);

        if (Exists(owner->GetID()))
            return;

        instance->Owner = owner;
        state.Scripts[owner->GetID()] = std::unique_ptr<ActorScript>(instance);

        CE_SCR_DBG("Registering script '%u'/'%s'", owner->GetID().Get(), owner->GetName().c_str());
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
