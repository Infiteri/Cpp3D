#include "World.h"
#include "Base.h"
#include "Core/Logger.h"

namespace Core
{
    static World::State state;

    void World::Init() { state.ActiveScene = nullptr; }

    void World::Shutdown()
    {
        for (auto scene : state.Scenes)
            delete scene.second;

        if (state.ActiveScene != nullptr)
            delete state.ActiveScene;

        state.ActiveScene = nullptr;
        state.Scenes.clear();
    }

    Scene *World::GetActive() { return state.ActiveScene; }

    Scene *World::Create(const std::string &name)
    {
        if (Exists(name))
        {
            CE_LOG("CE_SCENE", Warn, "Trying to create already created scene '%s'", name.c_str());
            return state.Scenes[name];
        }

        state.Scenes[name] = new Scene();
        state.Scenes[name]->SetName(name);
        // todo: think about names?
        return state.Scenes[name];
    }

    Scene *World::Get(const std::string &name)
    {
        if (!Exists(name))
            return Create(name);

        return state.Scenes[name];
    }

    void World::Remove(const std::string &name)
    {
        if (!Exists(name))
        {
            CE_LOG("CE_SCENE", Error, "Cannot delete inexisting scene '%s'", name.c_str());
            return;
        }

        delete state.Scenes[name];
        state.Scenes.erase(name);
    }

    bool World::Exists(const std::string &name)
    {
        return state.Scenes.find(name) != state.Scenes.end();
    }

    void World::Activate(const std::string &name)
    {
        if (state.ActiveScene != nullptr)
            DeactivateActive();

        if (!Exists(name))
        {
            CE_LOG("CE_SCENE", Error,
                   "Cannot activate scene '%s' as it doesnt exist. Make sure to call Create.",
                   name.c_str());
            return;
        }

        Scene *s = Get(name);
        if (!s)
            return;

        state.ActiveScene = s;
        state.ActiveScene->Start();
    }

    void World::DeactivateActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Stop();
        state.ActiveScene = nullptr;
    }

    void World::UpdateActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Update();
    }

    void World::RenderActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Render();
    }

} // namespace Core
