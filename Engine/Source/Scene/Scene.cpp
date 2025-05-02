#include "Scene.h"
#include "Core/Logger.h"

namespace Core
{
    Scene::Scene() { state = State::Uninitialzied; }

    Scene::~Scene() {}

    void Scene::Start()
    {
        if (state == State::Running)
        {
            CE_LOG("CE_SCENE", Warn,
                   "Attempting to call Start on scene that is already running, nothing done");
            return;
        }

        state = State::Started;

        for (auto ac : actors)
        {
            ac->Start();
        }
    }

    void Scene::Update()
    {
        if (state != State::Started && state != State::Running)
        {
            CE_LOG("CE_SCENE", Warn, "Calling update on scene that isn't started, nothing done.");
            return;
        }

        state = State::Running;

        for (auto ac : actors)
        {
            ac->Update();
        }
    }

    void Scene::Render()
    {
        if (state != State::Started && state != State::Running)
        {
            CE_LOG("CE_SCENE", Warn, "Calling render on scene that isn't started, nothing done.");
            return;
        }

        state = State::Running;

        for (auto ac : actors)
        {
            ac->Render();
        }
    }

    void Scene::Stop()
    {
        if (state == State::Stopped || state == State::Uninitialzied)
            return;

        for (auto ac : actors)
        {
            ac->Stop();
        }
    }

    Actor *Scene::CreateActor(const std::string &name)
    {
        Actor *a = new Actor(name);
        if (state == State::Started || state == State::Running)
            a->Start();

        actors.push_back(a);
        return a;
    }

} // namespace Core
