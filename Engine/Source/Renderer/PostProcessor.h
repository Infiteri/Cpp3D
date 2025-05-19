#pragma once

#include "Base.h"
#include "Renderer/Shader/Shader.h"
#include <unordered_map>
#include <vector>

namespace Core
{
    class CE_API PostProcessor
    {
    private:
        struct ShaderState
        {
            Shader *Shd;
            bool Enabled = true;
        };
        std::unordered_map<std::string, ShaderState> shaders;

        friend class Renderer;

    public:
        PostProcessor();
        ~PostProcessor();

        void Add(const std::string &name, bool enabled = true);
        void SetEnable(const std::string &name, bool enable);
        void Remove(const std::string &name);

        bool Exists(const std::string &name);

        std::vector<Shader *> GetEnabledShaders();
        Shader *Get(const std::string &name);
    };
} // namespace Core
