#pragma once

#include "Actor.h"
#include "Base.h"
#include "Core/UUID.h"
#include "Scene/SceneEnvironment.h"

#include <string>

namespace Core
{
    class CE_API Scene
    {
    private:
        // note: copied as states may differ also shouldn't be global in the namespace or anything
        // like that
        // todo: better system?
        enum class State
        {
            Uninitialzied, // note: First state, changes on any function call that matters ig
            Started,       // note: Started but not running yet, must be started in oreder to runb
            Running,       // note: Rendering or updating
            Stopped        // note: Stopped
        };
        State state = State::Uninitialzied;

        ActorList actors;

        SceneEnvironment environment;

        std::string name;

    public:
        Scene();
        ~Scene();

        inline ActorList &GetActors() { return actors; };
        inline SceneEnvironment *GetEnvironment() { return &environment; };
        inline std::string GetName() { return name; };

        void SetName(const std::string &n);

        void Start();
        void Update();
        void Render();
        void Stop();

        Actor *CreateActor(const std::string &name);
        void AddActor(Actor *a);

        void RemoveActor(const UUID &uuid, bool destroyInstance = false);

        void MoveActorInHierarchy(const UUID &uid, int newIndex);

        Actor *GetActor(const UUID &uuid);
        Actor *GetActorInAllHierarchy(const UUID &uuid);
    };
} // namespace Core
