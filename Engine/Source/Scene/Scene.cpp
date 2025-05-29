#include "Scene.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Renderer.h"
#include "Scene/Components/Components.h"
#include <algorithm>

namespace Core
{
    Scene::Scene() { state = State::Uninitialzied; }

    Scene::~Scene()
    {
        for (auto actor : actors)
            delete actor;

        actors.clear();
    }

    Scene *Scene::From(Scene *scene)
    {
        CE_VERIFY(scene) nullptr;

        Scene *out = new Scene();

        out->name = scene->name;
        out->environment.From(scene->environment);

        for (auto actor : scene->actors)
            out->AddActor(Actor::From(actor));

        return out;
    }

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

        ActivateSceneCamera();
    }

    void Scene::Update()
    {
        if (state != State::Started && state != State::Running)
        {
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
        if (state == State::Stopped)
            return;

        // todo: Better, note that its here due to some memory leak fixes, move somewhere else after
        // fixing editor layer
        Renderer::SetDirectioanlLightInstance(&environment.Light);
        Renderer::SetSkyInstance(&environment.Sky);

        state = State::Running;

        for (auto ac : actors)
        {
            ac->Render();
        }
    }

    void Scene::Stop()
    {
        if (state == State::Stopped || state == State::Uninitialzied)
        {
            CE_LOG("CE_SCENE", Warn,
                   "Calling stop on scene that is alreadyt stopped, nothing done.");
            return;
        }

        state = State::Stopped;

        for (auto ac : actors)
        {
            ac->Stop();
        }
    }

    void Scene::ActivateSceneCamera()
    {
        PerspectiveCameraComponent *camera = nullptr;

        for (auto a : actors)
            camera = a->GetCameraComponentInHierarchy(true);

        if (!camera)
            for (auto a : actors)
                camera = a->GetCameraComponentInHierarchy(false);

        CameraSystem::ActivatePerspectiveCameraInstance(&camera->Camera);
    }

    void Scene::SetName(const std::string &n) { name = n; }

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
