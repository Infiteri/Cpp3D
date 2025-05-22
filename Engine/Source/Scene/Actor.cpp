#include "Actor.h"
#include "Base.h"
#include "Core/Logger.h"

namespace Core
{
    Actor::Actor() { state = State::Uninitialzied; }

    Actor::Actor(const std::string &name) : name(name) { state = State::Uninitialzied; }

    Actor::~Actor()
    {
        for (auto comp : components)
            delete comp;

        for (auto child : children)
            delete child;

        components.clear();
    }

    void Actor::_CalculateTransformMatrices()
    {
        localMatrix = transform.GetMatrix();
        globalMatrix = parent ? (parent->GetGlobalMatrix() * localMatrix) : localMatrix;
    }

    void Actor::SetName(const std::string &newName) { name = newName; }

    Actor *Actor::CreateChild(const std::string &name)
    {
        Actor *a = new Actor(name);
        AddChild(a);
        return a;
    }

    void Actor::AddChild(Actor *childInstance)
    {
        CE_VERIFY(childInstance);

        childInstance->parent = this;
        children.push_back(childInstance);

        if (state == State::Running || state == State::Started)
            if (childInstance->state != State::Running && childInstance->state != State::Started)
                childInstance->Start();
    }

    void Actor::RemoveActor(const UUID &uuid, bool destroyInstance)
    {
        int index = 0;
        for (auto child : children)
        {
            if (child->GetID() == uuid)
            {
                child->parent = nullptr;
                if (destroyInstance)
                    delete child;

                children.erase(children.begin() + index);
                return;
            }

            index++;
        }
    }

    Actor *Actor::GetActor(const UUID &uuid)
    {
        if (id == uuid)
            return this;

        for (auto c : children)
            if (c->id == uuid)
                return c;

        return nullptr;
    }

    Actor *Actor::GetActorInAllHierarchy(const UUID &uuid)
    {
        Actor *a = GetActor(uuid);
        if (a)
            return a;

        for (auto *child : children)
        {
            Actor *result = child->GetActorInAllHierarchy(uuid);
            if (result != nullptr)
                return result;
        }

        return nullptr;
    }

    void Actor::Start()
    {
        if (state == State::Running)
        {
            CE_LOG("CE_SCENE", Warn,
                   "Attempting to call Start on actor that is already running, nothing done");
            return;
        }

        state = State::Started;

        for (auto comp : components)
        {
            comp->Start();
        }

        // todo: children vs components, which to init first
        for (auto child : children)
        {
            child->Start();
        }
    }

    void Actor::Update()
    {
        if (state != State::Started && state != State::Running)
        {
            CE_LOG("CE_SCENE", Warn, "Calling update on actor that isn't started, nothing done.");
            return;
        }

        state = State::Running;

        _CalculateTransformMatrices();

        for (auto comp : components)
        {
            comp->Update();
        }

        for (auto child : children)
        {
            child->Update();
        }
    }

    void Actor::Render()
    {
        if (state != State::Started && state != State::Running)
        {
            CE_LOG("CE_SCENE", Warn, "Calling render on actor that isn't started, nothing done.");
            return;
        }

        state = State::Running;

        for (auto comp : components)
        {
            comp->Render();
        }

        for (auto child : children)
        {
            child->Render();
        }
    }

    void Actor::Stop()
    {
        if (state == State::Stopped || state == State::Uninitialzied)
        {
            CE_LOG("CE_SCENE", Warn, "Calling stop on stopped actor, nothing done.");
            return;
        }

        state = State::Stopped;

        for (auto comp : components)
        {
            comp->Stop();
        }

        for (auto child : children)
        {
            child->Stop();
        }
    }

} // namespace Core
