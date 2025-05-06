#pragma once

#include "Base.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Scene/Components/Components.h"
#include <string>
#include <vector>

namespace Core
{
    typedef std::vector<Actor *> ActorList;

    class CE_API Actor
    {
    private:
        enum class State
        {
            Uninitialzied, // note: First state, changes on any function call that matters ig
            Started,       // note: Started but not running yet, must be started in oreder to runb
            Running,       // note: Rendering or updating
            Stopped        // note :Stopped
        };
        State state = State::Uninitialzied;

        std::string name;

        std::vector<Component *> components;

        Transform transform; // todo: local and global positions use the parent transforms when
                             // parents and children exist
        UUID id;

        ActorList children;
        Actor *parent = nullptr;

        Matrix4 localMatrix;
        Matrix4 globalMatrix;
        void _CalculateTransformMatrices();

    public:
        Actor();
        Actor(const std::string &name);
        ~Actor();

        inline Matrix4 &GetLocalMatrix() { return localMatrix; };
        inline Matrix4 &GetGlobalMatrix() { return globalMatrix; };
        inline Transform &GetTransform() { return transform; };

        inline ActorList &GetChildren() { return children; };
        inline Actor *GetParent() { return parent; };

        Actor *CreateChild(const std::string &name);
        void AddChild(Actor *childInstance);

        inline UUID GetID() { return id; };

        inline std::string GetName() { return name; }
        void SetName(const std::string &newName);

        void Start();
        void Update();
        void Render();
        void Stop();

        // note: Component related functions
        template <typename T, typename... Args> T *AddComponent(Args... args)
        {
            T *c = new T(args...);
            c->owner = this;

            if (state == State::Started || state == State::Running)
                c->Start(); // note: Component was added after the current scene was started, make
                            // sure to call start if needed

            components.push_back(c);
            return c;
        };

        /// note: Will return the first component of type T
        template <typename T> T *GetComponent()
        {
            for (auto comp : components)
            {
                T *tc = dynamic_cast<T *>(comp);
                if (tc)
                    return tc;
            }

            return nullptr;
        };

        template <typename T> std::vector<T *> GetComponents()
        {
            std::vector<T *> comps;
            for (auto comp : components)
            {
                T *tc = dynamic_cast<T *>(comp);
                if (tc)
                    comps.push_back(tc);
            }

            return comps;
        };

        template <typename T> bool HasComponent() { return GetComponent<T>() != nullptr; };

        /// note: Will remove the i'th component of type T, indicated by the index, leave at zero
        /// for the first component
        template <typename T> void RemoveComponent(int index = 0)
        {
            int currentIndex = -1;
            for (auto it = components.begin(); it != components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    currentIndex++;

                    if (currentIndex == index)
                    {
                        delete tc;
                        components.erase(it);
                        break;
                    }
                }
            }
        };

        template <typename T> void RemoveComponents()
        {
            for (auto it = components.begin(); it != components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    delete tc;
                    components.erase(it);
                }
            }
        };
    };
} // namespace Core
