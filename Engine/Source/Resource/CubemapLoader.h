#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Renderer/Texture/CubemapTexture.h"

namespace Core
{
    class CE_ED_API CubemapLoader : public CeSerializer
    {
    public:
        CubemapLoader() = default;
        ~CubemapLoader() = default;

        void Serialize(CubemapConfiguration &config, const std::string &name);
        void Serialize(CubemapConfiguration &config, YAML::Emitter &out);

        void Deserialize(CubemapConfiguration &config, const std::string &name);
        void Deserialize(CubemapConfiguration &config, YAML::Node name);
    };
} // namespace Core
