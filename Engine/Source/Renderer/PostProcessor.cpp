#include "PostProcessor.h"
#include "Renderer/Shader/ShaderSystem.h"

namespace Core
{
    PostProcessor::PostProcessor() {}

    PostProcessor::~PostProcessor() {}

    void PostProcessor::Add(const std::string &name, bool enabled)
    {
        if (Exists(name))
            shaders[name].Enabled = enabled;

        shaders[name] = {};
        shaders[name].Shd = ShaderSystem::Get(name);
        shaders[name].Enabled = enabled;
    }

    void PostProcessor::SetEnable(const std::string &name, bool enable)
    {
        if (!Exists(name))
            return;

        shaders[name].Enabled = enable;
    }

    void PostProcessor::Remove(const std::string &name)
    {
        if (!Exists(name))
            return;

        ShaderSystem::Remove(name);
        shaders.erase(name);
    }

    bool PostProcessor::Exists(const std::string &name)
    {
        return shaders.find(name) != shaders.end();
    }

    std::vector<Shader *> PostProcessor::GetEnabledShaders()
    {
        std::vector<Shader *> shds;

        for (auto s : shaders)
            if (s.second.Enabled)
                shds.push_back(s.second.Shd);

        return shds;
    }

    Shader *PostProcessor::Get(const std::string &name)
    {
        if (!Exists(name))
            return nullptr;
        return shaders[name].Shd;
    }
} // namespace Core
