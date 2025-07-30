#include "CeDataSerializer.h"
#include "CeSerializerUtils.h"
#include "Core/Data/CeData.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Math/Vector.h"
#include "yaml-cpp/emittermanip.h"
#include "yaml-cpp/yaml.h"

namespace Core
{
    CeDataSerializer::CeDataSerializer(CeDataSet *target) { set = target; }

    void CeDataSerializer::Serialize(const std::string &path)
    {
        YAML::Emitter out;
        CE_SERIALIZE_FIELD("DataSet", YAML::BeginSeq);
        Serialize(out);
        out << YAML::EndSeq;

        _SaveEmitter(out, path);
    }

    void CeDataSerializer::Serialize(YAML::Emitter &out)
    {
        for (auto &[first, data] : set->GetDataSet())
        {
            out << YAML::BeginMap;

            CE_SERIALIZE_FIELD("Name", data->GetName().c_str());
            CE_SERIALIZE_FIELD("Type", (int)data->GetType());

            switch (data->GetType())
            {
            case CeDataType::None:
            default:
                CE_WARN("Unknown data type when serializing ce data set");
                break;

            case CeDataType::Vector2:
            {
                auto d = data->As<Vector2>();
                SerializerUtils::SerializeVector2(*d, "Data", out);
            }
            break;

            case CeDataType::Vector3:
            {
                auto d = data->As<Vector3>();
                SerializerUtils::SerializeVector3(*d, "Data", out);
            }
            break;

            case CeDataType::Vector4:
            {
                auto d = data->As<Vector4>();
                SerializerUtils::SerializeVector4(*d, "Data", out);
            }
            break;

            case CeDataType::Color:
            {
                auto d = data->As<Color>();
                SerializerUtils::SerializeColor(*d, "Data", out);
            }
            break;

            case CeDataType::Float:
            {
                auto d = data->As<float>();
                CE_SERIALIZE_FIELD("Data", *d);
            }
            break;
            }
            out << YAML::EndMap;
        }
    }

    void CeDataSerializer::Deserialize(const std::string &path)
    {
        auto data = _LoadEmitterData(path);
        Deserialize(data["DataSet"]);
    }

    void CeDataSerializer::Deserialize(YAML::Node out)
    {
        for (auto data : out)
        {
            std::string name = data["Name"].as<std::string>();
            CeDataType type = (CeDataType)data["Type"].as<int>();

            switch (type)
            {
            case CeDataType::None:
            default:
                CE_WARN("Unknown / None data type when deserializing");
                break;

            case CeDataType::Vector2:
            {
                Vector2 v = SerializerUtils::DeserializeVector2(data["Data"]);
                Vector2 *d = new Vector2(v); // note that data doesnt get copied in the add function
                                             // so therefore we need to it this way in order to not
                                             // lose the vector2 after it gets freed from the stack
                set->Add(name, type, d);
            }
            break;

            case CeDataType::Vector3:
            {
                Vector3 v = SerializerUtils::DeserializeVector3(data["Data"]);
                Vector3 *d = new Vector3(v);
                set->Add(name, type, d);
            }
            break;
            case CeDataType::Vector4:
            {
                Vector4 v = SerializerUtils::DeserializeVector4(data["Data"]);
                Vector4 *d = new Vector4(v);
                set->Add(name, type, d);
            }
            break;
            case CeDataType::Color:
            {
                Color v = SerializerUtils::DeserializeColor(data["Data"]);
                Color *d = new Color(v);
                set->Add(name, type, d);
            }
            break;

            case CeDataType::Float:
            {
                float *f = new float(data["Data"].as<float>());
                set->Add(name, type, f);
            }
            break;
            }
        }
    }

} // namespace Core
