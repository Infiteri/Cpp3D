#pragma once

#include "Base.h"
#include "Scene/Actor.h"

namespace Core
{
    class CE_API ActorScript
    {
    public:
        Actor *Owner;

        ActorScript() {};
        ~ActorScript() {};

        virtual void OnStart() {};
        virtual void OnStop() {};

        virtual void OnUpdate() {};
    };
} // namespace Core

#define CE_DEFINE_ACTOR_SCRIPT(classname)                                                          \
    extern "C" __declspec(dllexport) Core::ActorScript *CE_CREATE_SCRIPT_PFN_##classname()         \
    {                                                                                              \
        return new classname();                                                                    \
    }
