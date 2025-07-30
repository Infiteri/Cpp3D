#include "ComponentSerializer.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Physics/Collider/Collider.h"
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
        CE_SERIALIZE_COMPONENT_CALLBACK(PointLight);
        CE_SERIALIZE_COMPONENT_CALLBACK(SpotLight);
        CE_SERIALIZE_COMPONENT_CALLBACK(PerspectiveCamera);
        CE_SERIALIZE_COMPONENT_CALLBACK(Script);
        CE_SERIALIZE_COMPONENT_CALLBACK(RigidBody);
        CE_SERIALIZE_COMPONENT_CALLBACK(StaticBody);
        CE_SERIALIZE_COMPONENT_CALLBACK(Collider);
    }

    void ComponentSerializer::_SerialzieComponentCount(YAML::Emitter &out)
    {
        CE_GET_COMPONENT_COUNT(Mesh);
        CE_GET_COMPONENT_COUNT(PointLight);
        CE_GET_COMPONENT_COUNT(SpotLight);
        CE_GET_COMPONENT_COUNT(PerspectiveCamera);
        CE_GET_COMPONENT_COUNT(Script);
        CE_GET_COMPONENT_COUNT(RigidBody);
        CE_GET_COMPONENT_COUNT(StaticBody);
        CE_GET_COMPONENT_COUNT(Collider);

        CE_SERIALIZE_FIELD("MeshComponentCount", count.Mesh);
        CE_SERIALIZE_FIELD("PointLightComponentCount", count.PointLight);
        CE_SERIALIZE_FIELD("SpotLightComponentCount", count.SpotLight);
        CE_SERIALIZE_FIELD("PerspectiveCameraComponentCount", count.PerspectiveCamera);
        CE_SERIALIZE_FIELD("ScriptComponentCount", count.Script);
        CE_SERIALIZE_FIELD("RigidBodyComponentCount", count.RigidBody);
        CE_SERIALIZE_FIELD("StaticBodyComponentCount", count.StaticBody);
        CE_SERIALIZE_FIELD("ColliderComponentCount", count.Collider);
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

    void ComponentSerializer::_SerializePointLightComponent(PointLightComponent *c, int index,
                                                            YAML::Emitter &out)
    {
        out << YAML::Key << "PointLightComponent " + std::to_string(index);
        out << YAML::BeginMap;

        SerializerUtils::SerializeVector3(c->Light.Specular, "Specular", out);
        SerializerUtils::SerializeColor(c->Light.Color, "Color", out);

        CE_SERIALIZE_FIELD("Constant", c->Light.Constant);
        CE_SERIALIZE_FIELD("Linear", c->Light.Linear);
        CE_SERIALIZE_FIELD("Quadratic", c->Light.Quadratic);
        CE_SERIALIZE_FIELD("Intensity", c->Light.Intensity);
        CE_SERIALIZE_FIELD("Radius", c->Light.Radius);

        out << YAML::EndMap;
    }

    void ComponentSerializer::_SerializeSpotLightComponent(SpotLightComponent *c, int index,
                                                           YAML::Emitter &out)
    {
        out << YAML::Key << "SpotLightComponent " + std::to_string(index);
        out << YAML::BeginMap;

        SerializerUtils::SerializeVector3(c->Light.Specular, "Specular", out);
        SerializerUtils::SerializeVector3(c->Light.Diffuse, "Diffuse", out);
        SerializerUtils::SerializeVector3(c->Light.Direction, "Direction", out);
        SerializerUtils::SerializeColor(c->Light.Color, "Color", out);

        CE_SERIALIZE_FIELD("Constant", c->Light.Constant);
        CE_SERIALIZE_FIELD("Linear", c->Light.Linear);
        CE_SERIALIZE_FIELD("Quadratic", c->Light.Quadratic);
        CE_SERIALIZE_FIELD("CutOff", c->Light.CutOff);
        CE_SERIALIZE_FIELD("OuterCutOff", c->Light.OuterCutOff);

        out << YAML::EndMap;
    }

    void ComponentSerializer::_SerializePerspectiveCameraComponent(PerspectiveCameraComponent *c,
                                                                   int index, YAML::Emitter &out)
    {
        out << YAML::Key << "PerspectiveCameraComponent " + std::to_string(index);
        out << YAML::BeginMap;

        CE_SERIALIZE_FIELD("FOV", c->FOV);
        CE_SERIALIZE_FIELD("Near", c->Near);
        CE_SERIALIZE_FIELD("Far", c->Far);
        CE_SERIALIZE_FIELD("IsPrimary", c->IsPrimary);

        out << YAML::EndMap;
    }

    void ComponentSerializer::_SerializeScriptComponent(ScriptComponent *c, int index,
                                                        YAML::Emitter &out)
    {
        out << YAML::Key << "ScriptComponent " + std::to_string(index);
        out << YAML::BeginMap;

        CE_SERIALIZE_FIELD("ClassName", c->ClassName.c_str());

        out << YAML::EndMap;
    }

    void ComponentSerializer::Deserialize(YAML::Node &node)
    {
        CE_DESERIALIZE_COMPONENT("MeshComponent", _DeserializeMeshComponent);
        CE_DESERIALIZE_COMPONENT("PointLightComponent", _DeserializePointLightComponent);
        CE_DESERIALIZE_COMPONENT("SpotLightComponent", _DeserializeSpotLightComponent);
        CE_DESERIALIZE_COMPONENT("PerspectiveCameraComponent",
                                 _DeserializePerspectiveCameraComponent);

        CE_DESERIALIZE_COMPONENT("ScriptComponent", _DeserializeScriptComponent);
        CE_DESERIALIZE_COMPONENT("RigidBodyComponent", _DeserializeRigidBodyComponent);
        CE_DESERIALIZE_COMPONENT("StaticBodyComponent", _DeserializeStaticBodyComponent);
        CE_DESERIALIZE_COMPONENT("ColliderComponent", _DeserializeColliderComponent);
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

    void ComponentSerializer::_DeserializePointLightComponent(YAML::Node node)
    {
        auto pl = &actor->AddComponent<PointLightComponent>()->Light;

        pl->Color = SerializerUtils::DeserializeColor(node["Color"]);
        pl->Specular = SerializerUtils::DeserializeVector3(node["Specular"]);

        pl->Constant = node["Constant"].as<float>();
        pl->Linear = node["Linear"].as<float>();
        pl->Quadratic = node["Quadratic"].as<float>();
        pl->Radius = node["Radius"].as<float>();
        pl->Intensity = node["Intensity"].as<float>();
    }

    void ComponentSerializer::_DeserializeSpotLightComponent(YAML::Node node)
    {
        auto pl = &actor->AddComponent<SpotLightComponent>()->Light;

        pl->Color = SerializerUtils::DeserializeColor(node["Color"]);
        pl->Specular = SerializerUtils::DeserializeVector3(node["Specular"]);
        pl->Diffuse = SerializerUtils::DeserializeVector3(node["Diffuse"]);
        pl->Direction = SerializerUtils::DeserializeVector3(node["Direction"]);

        pl->Constant = node["Constant"].as<float>();
        pl->Linear = node["Linear"].as<float>();
        pl->Quadratic = node["Quadratic"].as<float>();
        pl->CutOff = node["CutOff"].as<float>();
        pl->OuterCutOff = node["OuterCutOff"].as<float>();
    }

    void ComponentSerializer::_DeserializePerspectiveCameraComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<PerspectiveCameraComponent>();

        c->FOV = node["FOV"].as<float>();
        c->Near = node["Near"].as<float>();
        c->Far = node["Far"].as<float>();
        c->IsPrimary = node["IsPrimary"].as<bool>();
    }

    void ComponentSerializer::_DeserializeScriptComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<ScriptComponent>();
        c->ClassName = node["ClassName"].as<std::string>();
    }

    void ComponentSerializer::_SerializeRigidBodyComponent(RigidBodyComponent *c, int index,
                                                           YAML::Emitter &out)
    {
        out << YAML::Key << "RigidBodyComponent " + std::to_string(index);
        out << YAML::BeginMap;

        CE_SERIALIZE_FIELD("Mass", c->Config.Mass);
        CE_SERIALIZE_FIELD("AngularDamping", c->Config.AngularDamping);
        CE_SERIALIZE_FIELD("LinearDamping", c->Config.LinearDamping);
        CE_SERIALIZE_FIELD("Friction", c->Config.Friction);
        CE_SERIALIZE_FIELD("Restitution", c->Config.Restitution);

        out << YAML::EndMap;
    }

    void ComponentSerializer::_DeserializeRigidBodyComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<RigidBodyComponent>();

        c->Config.Mass = node["Mass"].as<float>();
        c->Config.AngularDamping = node["AngularDamping"].as<float>();
        c->Config.LinearDamping = node["LinearDamping"].as<float>();
        c->Config.Friction = node["Friction"].as<float>();
        c->Config.Restitution = node["Restitution"].as<float>();
    }

    void ComponentSerializer::_SerializeStaticBodyComponent(StaticBodyComponent *c, int index,
                                                            YAML::Emitter &out)
    {
        out << YAML::Key << "StaticBodyComponent " + std::to_string(index);
        out << YAML::BeginMap;

        CE_SERIALIZE_FIELD("AngularDamping", c->Config.AngularDamping);
        CE_SERIALIZE_FIELD("LinearDamping", c->Config.LinearDamping);
        CE_SERIALIZE_FIELD("Friction", c->Config.Friction);
        CE_SERIALIZE_FIELD("Restitution", c->Config.Restitution);

        out << YAML::EndMap;
    }

    void ComponentSerializer::_DeserializeStaticBodyComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<StaticBodyComponent>();

        c->Config.AngularDamping = node["AngularDamping"].as<float>();
        c->Config.LinearDamping = node["LinearDamping"].as<float>();
        c->Config.Friction = node["Friction"].as<float>();
        c->Config.Restitution = node["Restitution"].as<float>();
    }

    void ComponentSerializer::_SerializeColliderComponent(ColliderComponent *c, int index,
                                                          YAML::Emitter &out)
    {
        out << YAML::Key << "ColliderComponent " + std::to_string(index);
        out << YAML::BeginMap;

        ColliderType type = c->Config.Type;

        CE_SERIALIZE_FIELD("Type", (int)type);

        switch (type)
        {
        default:
            break;

        case ColliderType::Box:
        {
            CE_SERIALIZE_FIELD("Width", c->Config.BoxType.Width);
            CE_SERIALIZE_FIELD("Height", c->Config.BoxType.Height);
            CE_SERIALIZE_FIELD("Depth", c->Config.BoxType.Depth);
        }
        break;
        }

        out << YAML::EndMap;
    }

    void ComponentSerializer::_DeserializeColliderComponent(YAML::Node node)
    {
        auto c = actor->AddComponent<ColliderComponent>();

        ColliderType type = (ColliderType)(node["Type"].as<int>());
        c->Config.Type = type;
        switch (type)
        {
        default:
            CE_WARN("Unknown type when deserializing");
            // todo: warnings?
            break;

        case ColliderType::Box:
            c->Config.BoxType.Width = node["Width"].as<float>();
            c->Config.BoxType.Height = node["Height"].as<float>();
            c->Config.BoxType.Depth = node["Depth"].as<float>();
            break;
        }
    }

} // namespace Core
