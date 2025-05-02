#pragma once

#include "Actor.h"
#include "Base.h"

#include <string>
#include <vector>

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
            Stopped        // note :Stopped
        };
        State state = State::Uninitialzied;

        std::vector<Actor *> actors;

    public:
        Scene();
        ~Scene();

        void Start();
        void Update();
        void Render();
        void Stop();

        Actor *CreateActor(const std::string &name);
    };
} // namespace Core
