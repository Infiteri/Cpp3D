#include "Scene.h"
#include "Core/Logger.h"
#include "Renderer/Renderer.h"
#include <algorithm>

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

        environment.Sky.SetSkyboxMode("Cubemap.ce_cubemap");

        Renderer::SetDirectioanlLightInstance(&environment.Light);
        Renderer::SetSkyInstance(&environment.Sky);
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
        AddActor(a);
        return a;
    }

    void Scene::AddActor(Actor *a)
    {
        if (state == State::Started || state == State::Running)
            a->Start();

        actors.push_back(a);
    }

    void Scene::MoveActorInHierarchy(const UUID &uid, int newIndex)
    {
        Actor *actorToMove = GetActor(uid);

        if (!actorToMove)
            return;

        if (newIndex < 0 || newIndex >= actors.size())
            return;
        auto actorIterator = std::find(actors.begin(), actors.end(), actorToMove);

        if (actorIterator != actors.end())
        {
            actors.erase(actorIterator);
            actors.insert(actors.begin() + newIndex, actorToMove);
        }
    }

    void Scene::RemoveActor(const UUID &uuid, bool destroyInstance)
    {
        int index = 0;
        for (auto a : actors)
        {
            if (a->GetID() == uuid)
            {
                if (destroyInstance)
                    delete a;

                actors.erase(actors.begin() + index);
                return;
            }

            index++;
        }
    }

    Actor *Scene::GetActor(const UUID &uuid)
    {
        for (auto a : actors)
            if (a->GetID() == uuid)
                return a;

        return nullptr;
    }

    Actor *Scene::GetActorInAllHierarchy(const UUID &uuid)
    {
        Actor *a = GetActor(uuid);

        if (!a)
        {
            if (a)
                return a;

            for (auto c : actors)
                a = c->GetActorInAllHierarchy(uuid);
        }

        return a;
    }

} // namespace Core
