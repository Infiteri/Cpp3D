#include "CubemapLoader.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    static std::string _DeserializeString(YAML::Node n, const char *field)
    {
        if (!n[field])
            return "";
        return n[field].as<std::string>();
    }

    void CubemapLoader::Serialize(CubemapConfiguration &config, const std::string &name)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Cubemap", YAML::BeginMap);
        Serialize(config, out);
        out << YAML::EndMap;

        _SaveEmitter(out, name);
    }

    void CubemapLoader::Serialize(CubemapConfiguration &config, YAML::Emitter &out)
    {
        CE_SERIALIZE_FIELD("Right", config.Right.c_str());
        CE_SERIALIZE_FIELD("Left", config.Left.c_str());
        CE_SERIALIZE_FIELD("Top", config.Top.c_str());
        CE_SERIALIZE_FIELD("Bottom", config.Bottom.c_str());
        CE_SERIALIZE_FIELD("Front", config.Front.c_str());
        CE_SERIALIZE_FIELD("Back", config.Back.c_str());
    }

    void CubemapLoader::Deserialize(CubemapConfiguration &config, const std::string &name)
    {
        YAML::Node data = _LoadEmitterData(name);

        if (!data)
        {
            CE_ERROR("Unable to deserialize cubemap '%s'", name.c_str());
            return;
        }

        if (!data["Cubemap"])
        {
            CE_ERROR("Unable to deserialize cubemap '%s', cubemap root not found", name.c_str());
            return;
        }

        auto cm = data["Cubemap"];
        Deserialize(config, cm);
    }

    void CubemapLoader::Deserialize(CubemapConfiguration &config, YAML::Node cm)
    {
        if (!cm)
        {
            CE_ERROR("Unable to deserialize cubemap from node as it is not valid");
            return;
        }

        config.Right = _DeserializeString(cm, "Right");
        config.Left = _DeserializeString(cm, "Left");
        config.Top = _DeserializeString(cm, "Top");
        config.Bottom = _DeserializeString(cm, "Bottom");
        config.Front = _DeserializeString(cm, "Front");
        config.Back = _DeserializeString(cm, "Back");
    }

} // namespace Core
