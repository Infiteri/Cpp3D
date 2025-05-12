#include "MaterialLoader.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    static void SerializeTexture(Texture2D *tex, const char *field, YAML::Emitter &out)
    {
        out << YAML::Key << field << YAML::Value << YAML::BeginMap;
        CE_SERIALIZE_FIELD("FilePath", tex->GetImagePath().c_str());
        out << YAML::EndMap;
    }

    static void DeserializeTexture(YAML::Node node, const char *field,
                                   Material::Configuration &config)
    {
        std::string &targetPath = config.ColorTexture;

        if (node[field])
            targetPath = node[field]["FilePath"].as<std::string>();
    }

    void MaterialLoader::Serialize(const std::string &path, Material *material, MaterialType type)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Material", "Value");
        Serialize(out, material, type);
        out << YAML::EndMap;

        _SaveEmitter(out, path);
    }

    void MaterialLoader::Serialize(YAML::Emitter &out, Material *material, MaterialType type)
    {
        auto mat = material;
        CE_SERIALIZE_FIELD("Type", (int)type);

        switch (type)
        {
        case MaterialType::Default:
        default:
            break;

        case MaterialType::Config:
            SerializerUtils::SerializeColor(mat->GetColor(), "Color", out);

            if (mat->GetColorTexture())
                SerializeTexture(mat->GetColorTexture(), "ColorTexture", out);
            else
                CE_ERROR("Material Color Texture should never be nullptr.");

            break;

        case MaterialType::File:
            CE_SERIALIZE_FIELD("File", mat->GetName().c_str());
            break;
        }
    }

    void MaterialLoader::Deserialize(const std::string &path, Material::Configuration &material)
    {
        YAML::Node data = _LoadEmitterData(path);

        Deserialize(data, material);
    }

    void MaterialLoader::Deserialize(YAML::Node node, Material::Configuration &material)
    {
        material.Color = SerializerUtils::DeserializeColor(node["Color"]);
        DeserializeTexture(node, "ColorTexture", material);
    }
} // namespace Core
