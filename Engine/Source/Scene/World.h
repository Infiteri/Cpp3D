#pragma once

#include "Base.h"
#include "Scene.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class CE_API World
    {
    public:
        struct State
        {
            Scene *ActiveScene;

            std::unordered_map<std::string, Scene *> Scenes;
        };

    public:
        World() {};
        ~World() {};

        static void Init();
        static void Shutdown();

        static Scene *Create(const std::string &name);

        static Scene *Get(const std::string &name);

        static void Remove(const std::string &name);

        static bool Exists(const std::string &name);

        static void Activate(const std::string &name);

        static void DeactivateActive();

        // note: scene specific functions to make life easier
        static void UpdateActive();
        static void RenderActive();
    };
} // namespace Core
