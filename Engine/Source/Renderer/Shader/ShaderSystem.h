#pragma once

#include "Base.h"
#include "Renderer/Shader/Shader.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class ShaderSystem
    {
    public:
        struct State
        {
            std::string EngineAssetPath = "EngineAssest/Shaders";
            std::unordered_map<std::string, Shader> shaders;
        };

    public:
        ShaderSystem() {};
        ~ShaderSystem() {};

        // will autload all shaders used by engine
        static void Init();
        static void Shutdown();

        static void Load(const std::string &name);
        static Shader *Get(const std::string &name);
        static bool Exist(const std::string &name);
        static void Remove(const std::string &name);

        static Shader *GetEngineResource(const std::string &name);
    };
} // namespace Core
