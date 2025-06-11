#include "World.h"
#include "Base.h"
#include "Core/Logger.h"

namespace Core
{
    static World::State state;

    void World::Init() { state.ActiveScene = nullptr; }

    void World::Shutdown() { ClearScenes(); }

    void World::Editor_ActivateFromInstance(Scene *scene)
    {
        if (state.ActiveScene != nullptr)
            DeactivateActive();

        if (!scene)
            return;

        state.ActiveScene = scene;
        state.ActiveScene->Start();
    }

    void World::Editor_CopyToActive(Scene *scene)
    {
        CE_VERIFY(scene);

        if (state.ActiveScene)
            delete state.ActiveScene;

        state.ActiveScene = Scene::From(scene);
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
    }

    void World::DeactivateActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Stop();
        state.ActiveScene = nullptr;
    }

    void World::ClearScenes()
    {
        for (auto scene : state.Scenes)
        {
            scene.second->Stop();
            delete scene.second;
            scene.second = nullptr;
        }

        if (state.ActiveScene != nullptr)
        {
            state.ActiveScene->Stop();
            delete state.ActiveScene;
        }

        state.ActiveScene = nullptr;
        state.Scenes.clear();
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

    void World::StartActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Start();
    }

    void World::StopActive()
    {
        CE_VERIFY(state.ActiveScene);

        state.ActiveScene->Stop();
    }
} // namespace Core
