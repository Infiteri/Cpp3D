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

    private:
        friend class EditorLayer;
        static void Editor_ActivateFromInstance(Scene *scene);
        static void Editor_CopyToActive(Scene *scene);

    public:
        World() {};
        ~World() {};

        static void Init();
        static void Shutdown();

        static Scene *Create(const std::string &name);

        static Scene *GetActive();

        static Scene *Get(const std::string &name);

        static void Remove(const std::string &name);

        static bool Exists(const std::string &name);

        static void Activate(const std::string &name);

        static void DeactivateActive();

        // note: scene specific functions to make life easier
        static void UpdateActive();
        static void RenderActive();

        static void StartActive();
        static void StopActive();
    };
} // namespace Core
