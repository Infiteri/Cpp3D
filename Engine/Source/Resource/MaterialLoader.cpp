#include "MaterialLoader.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

// hack: dear ttv, this whole shit requires a rework, please get your ass on this file ASAP you
// idiot,
// yours trully, ttv

namespace Core
{
    static void SerializeTexture(const std::string &filepath, const char *field, YAML::Emitter &out)
    {
        out << YAML::Key << field << YAML::Value << YAML::BeginMap;
        CE_SERIALIZE_FIELD("FilePath", filepath.c_str());
        out << YAML::EndMap;
    }

    static void SerializeTexture(Texture2D *tex, const char *field, YAML::Emitter &out)
    {
        CE_VERIFY(tex);
        SerializeTexture(tex->GetImagePath(), field, out);
    }

    static void DeserializeTexture(YAML::Node node, const char *field, std::string &targetPath)
    {
        if (node[field])
            targetPath = node[field]["FilePath"].as<std::string>();
    }

    // todo: FIX THE WRONG FILE FORMAT PLEASE IT SHOULD ALL BE UNDER THE MAP
    void MaterialLoader::Serialize(const std::string &path, Material *material)
    {
        Serialize(path, material->GetState());
    }

    void MaterialLoader::Serialize(YAML::Emitter &out, Material *material)
    {
        Serialize(out, material->GetState());
    }

    void MaterialLoader::Serialize(const std::string &name, Material::Configuration &config)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        Serialize(out, config);
        out << YAML::EndMap;

        _SaveEmitter(out, name);
    }

    void MaterialLoader::Serialize(YAML::Emitter &out, Material::Configuration &config)
    {
        SerializerUtils::SerializeColor(config.Color, "Color", out);

        CE_SERIALIZE_FIELD("Metallic", config.Metallic);
        CE_SERIALIZE_FIELD("Roughness", config.Roughness);
        CE_SERIALIZE_FIELD("AO", config.AO);

        SerializeTexture(config.ColorTexture, "ColorTexture", out);
        SerializeTexture(config.NormalTexture, "NormalTexture", out);
    }

    void MaterialLoader::Deserialize(const std::string &path, Material::Configuration &material)
    {
        YAML::Node data = _LoadEmitterData(path);

        Deserialize(data, material);
    }

    void MaterialLoader::Deserialize(YAML::Node node, Material::Configuration &material)
    {
        material.Color = SerializerUtils::DeserializeColor(node["Color"]);
        material.Metallic = node["Metallic"].as<float>();
        material.Roughness = node["Roughness"].as<float>();
        material.AO = node["AO"].as<float>();
        DeserializeTexture(node, "ColorTexture", material.ColorTexture);
        DeserializeTexture(node, "NormalTexture", material.NormalTexture);
    }
} // namespace Core
