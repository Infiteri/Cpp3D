#pragma once

#include "Base.h"
#include "Core/Serializer/CeSerializer.h"
#include "Renderer/Material/Material.h"

#include <string>

namespace Core
{
    class CE_ED_API MaterialLoader : public CeSerializer
    {
    public:
        MaterialLoader() {};
        ~MaterialLoader() {};

        void Serialize(const std::string &path, Material *material);
        void Deserialize(const std::string &path, Material::Configuration &material);

        void Serialize(YAML::Emitter &out, Material *material);
        void Deserialize(YAML::Node node, Material::Configuration &material);
    };
} // namespace Core
