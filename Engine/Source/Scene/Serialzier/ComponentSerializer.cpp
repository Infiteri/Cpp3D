#include "ComponentSerializer.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Material/Material.h"
#include "Resource/MaterialLoader.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

#define CE_GET_COMPONENT_COUNT(type) count.type = actor->GetComponents<type##Component>().size()

#define CE_SERIALIZE_COMPONENT_CALLBACK(type)                                                      \
    {                                                                                              \
        auto list = actor->GetComponents<type##Component>();                                       \
        for (int i = 0; i < list.size(); i++)                                                      \
            _Serialize##type##Component(list[i], i, out);                                          \
    }

#define CE_DESERIALIZE_COMPONENT(name, fun)                                                        \
    for (int i = 0; i < GetNodeCount(name, node); i++)                                             \
    fun(node[std::string(name) + std::string(" ") + std::to_string(i)])

namespace Core
{
    static int GetNodeCount(const std::string &name, YAML::Node node)
    {
        std::string fmt = name + "Count";
        if (node[fmt])
            return node[fmt].as<int>();
        else
            return 0;
    };

    static void MeshSerializeTexture(Texture2D *tex, const char *field, YAML::Emitter &out)
    {
        out << YAML::Key << field << YAML::Value << YAML::BeginMap;
        CE_SERIALIZE_FIELD("FilePath", tex->GetImagePath().c_str());
        out << YAML::EndMap;
    }

    ComponentSerializer::ComponentSerializer(Actor *target) { actor = target; }

    void ComponentSerializer::Serialize(YAML::Emitter &out)
    {
        // note: Because of multiple components, each actor serializes how many components of a
        // certain type are present (e.g. 3 mesh components, 2 tag components ...)
        // note:to do this, we load all component count data in a structure and that is the first
        // thing that is serialized here

        // Serialize component count here
        _SerialzieComponentCount(out);

        // Serialize each component
        CE_SERIALIZE_COMPONENT_CALLBACK(Mesh);
    }

    void ComponentSerializer::_SerialzieComponentCount(YAML::Emitter &out)
    {
        CE_GET_COMPONENT_COUNT(Mesh);

        CE_SERIALIZE_FIELD("MeshComponentCount", count.Mesh);
    }

    void ComponentSerializer::_SerializeMeshComponent(MeshComponent *mesh, int index,
                                                      YAML::Emitter &out)
    {
        out << YAML::Key << "MeshComponent " + std::to_string(index);
        out << YAML::BeginMap;

        // Material
        {
            out << YAML::Key << "Material" << YAML::Value << YAML::BeginMap;
            auto mat = mesh->GetMesh()->GetMaterial();
            CE_SERIALIZE_FIELD("Type", (int)mesh->GetMesh()->GetMaterialType());

            switch (mesh->GetMesh()->GetMaterialType())
            {
            case MaterialType::Default:
            default:
                break;

            case MaterialType::Config:
            {
                MaterialLoader loader;
                loader.Serialize(out, mesh->GetMesh()->GetMaterial());
            }
            break;

            case MaterialType::File:
                CE_SERIALIZE_FIELD("File", mat->GetName().c_str());
                break;
            }

            out << YAML::EndMap;
        }
        // Geometry
        {
            out << YAML::Key << "Geometry" << YAML::Key << YAML::BeginMap;
            auto geo = mesh->GetMesh()->GetGeometry();
            CE_SERIALIZE_FIELD("Type", mesh->GetMesh()->GetGeometry()->GetType());
            switch (geo->GetType())
            {
            case Geometry::None:
            default:
                break;

            case Geometry::Box:
            {
                BoxGeometry *box = (BoxGeometry *)geo;
                SerializerUtils::SerializeVector3({box->Width, box->Height, box->Depth}, "Size",
                                                  out);
            }
            break;
            }

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void ComponentSerializer::Deserialize(YAML::Node &node)
    {
        CE_DESERIALIZE_COMPONENT("MeshComponent", _DeserializeMeshComponent);
    }

    void ComponentSerializer::_DeserializeMeshComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<MeshComponent>();

        {
            auto mc = node["Material"];
            MaterialType type = (MaterialType)mc["Type"].as<int>();
            switch (type)
            {
            case MaterialType::Default:
            default:
                c->GetMesh()->SetMaterialToDefault();
                break;

            case MaterialType::Config:
            {
                // todo: Refactor in a way to allow for the same code to be used for material file
                // assets
                Material::Configuration config;
                MaterialLoader loader;
                loader.Deserialize(mc, config);
                c->GetMesh()->SetMaterial(config);
            }
            break;

            case MaterialType::File:
                std::string name = mc["File"].as<std::string>();
                c->GetMesh()->SetMaterial(name);
                break;
            }
        }

        {
            auto gc = node["Geometry"];
            Geometry::Type type = (Geometry::Type)gc["Type"].as<int>();

            switch (type)
            {
            case Geometry::Box:
            {
                BoxGeometry *g =
                    new BoxGeometry(gc["Size"][0].as<float>(), gc["Size"][1].as<float>(),
                                    gc["Size"][2].as<float>());

                c->GetMesh()->SetGeometry(g);
            }
            break;

            default:
                break;
            }
        }
    }

} // namespace Core
