#include "PostProcessor.h"
#include "Core/Logger.h"
#include "Renderer/Shader/ShaderSystem.h"

namespace Core
{
    PostProcessor::PostProcessor() {}

    PostProcessor::~PostProcessor() {}

    void PostProcessor::Add(const std::string &name, bool enabled)
    {
        if (Exists(name))
        {
            shaders[name].Enabled = enabled;
            return;
        }

        shaders[name] = {};
        shaders[name].Shd = new Shader(name);
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

        delete shaders[name].Shd;
        shaders.erase(name);
    }

    bool PostProcessor::Exists(const std::string &name)
    {
        return shaders.find(name) != shaders.end();
    }

    void PostProcessor::ReloadShaders()
    {
        for (auto &[name, entry] : shaders)
        {
            delete entry.Shd;
            entry.Shd = new Shader(name);
        }
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
