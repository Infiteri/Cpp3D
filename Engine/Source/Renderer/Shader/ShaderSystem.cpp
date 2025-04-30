#include "ShaderSystem.h"
#include "Core/Logger.h"

namespace Core
{
    static ShaderSystem::State state;

    void ShaderSystem::Init()
    {
        // todo: better loading of engine resources? autoloading
        GetEngineResource("Object.glsl");
        GetEngineResource("Screen.glsl");
    }

    void ShaderSystem::Shutdown() { state.shaders.clear(); }

    void ShaderSystem::Load(const std::string &name)
    {
        if (Exist(name))
        {
            CE_LOG("CE_RENDER", Warn, "Unable to Load shader, already exists ('%s')", name.c_str());
            return;
        }

        state.shaders[name].Load(name); // adding it is funny todo: make sure it workw
    }

    Shader *ShaderSystem::Get(const std::string &name)
    {
        if (!Exist(name))
            Load(name);

        return &state.shaders[name];
    }

    bool ShaderSystem::Exist(const std::string &name)
    {
        return state.shaders.find(name) != state.shaders.end();
    }

    void ShaderSystem::Remove(const std::string &name)
    {
        if (!Exist(name))
        {
            CE_LOG("CE_RENDER", Warn, "Unable to Remove shader, doesn't exist ('%s')",
                   name.c_str());
            return;
        }

        state.shaders.erase(name);
    }

    Shader *ShaderSystem::GetEngineResource(const std::string &name)
    {
        return Get(state.EngineAssetPath + "/" + name);
    }
} // namespace Core
