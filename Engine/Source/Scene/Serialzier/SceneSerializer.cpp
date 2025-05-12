#include "SceneSerializer.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Scene/Actor.h"
#include "Scene/Serialzier/ComponentSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    SceneSerializer::SceneSerializer(Scene *target) : scene(target) {}

    void SceneSerializer::Serialize(const std::string &filepath)
    {
        CE_VERIFY(scene);

        YAML::Emitter emmiter;
        emmiter << YAML::BeginMap;
        CE_SERIALIZE_FIELD_TO(emmiter, "Scene", "Value");

        CE_SERIALIZE_FIELD_TO(emmiter, "Actors", YAML::BeginSeq);

        // serialize Actors
        ActorList actors = scene->GetActors();
        for (auto actor : actors)
            _SerializeActor(actor, emmiter);

        emmiter << YAML::EndMap;

        _SaveEmitter(emmiter, filepath);
    }

    void SceneSerializer::_SerializeActor(Actor *a, YAML::Emitter &out)
    {
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Actor", a->GetName().c_str());

        // General information
        CE_SERIALIZE_FIELD("Name", a->GetName().c_str());
        CE_SERIALIZE_FIELD("UUID", a->GetID().Get());
        CE_SERIALIZE_FIELD("Parent",
                           (a->GetParent() != nullptr ? a->GetParent()->GetID().Get() : 0));

        // todo: Format?
        // [POSX POSY POSZ ROTX ROTY ROTZ SCAX SCAY SCAZ]
        // -- OR --
        // Position: [X Y Z], Rotation [X Y Z], Scale: [X Y Z]
        {
            auto t = a->GetTransform();
            out << YAML::Key << "Transform";

#if 1
            out << YAML::Value << YAML::Flow;
            out << YAML::BeginSeq << t.Position.x << t.Position.y << t.Position.z;
            out << t.Rotation.x << t.Rotation.y << t.Rotation.z;
            out << t.Scale.x << t.Scale.y << t.Scale.z << YAML::EndSeq;
#else
            out << YAML::Value << YAML::BeginMap;
            SerializerUtils::SerializeVector3(t.Position, "Position", out);
            SerializerUtils::SerializeVector3(t.Rotation, "Rotation", out);
            SerializerUtils::SerializeVector3(t.Scale, "Scale", out);
            out << YAML::Value << YAML::EndMap;
#endif
        }

        // Serialize components
        ComponentSerializer componentSerializer(a);
        componentSerializer.Serialize(out);

        out << YAML::EndMap;

        // Serialize children
        for (auto child : a->GetChildren())
            _SerializeActor(child, out);
    }

    void SceneSerializer::Deserialize(const std::string &filepath)
    {
        YAML::Node data = _LoadEmitterData(filepath);

        if (!data || !data["Scene"])
        {
            CE_LOG("CE_SCENE", Error, "Unable to get scene data for file '%s'", filepath.c_str());
            return;
        }

        auto actors = data["Actors"];
        if (actors)
        {
            for (auto actor : actors)
            {
                Actor *a = new Actor();
                a->SetName(actor["Name"].as<std::string>());
                a->id = actor["UUID"].as<u64>();

                // note: Transform if the format is different change here as well
                {
                    auto t = actor["Transform"];
                    a->GetTransform().Position = {t[0].as<float>(), t[1].as<float>(),
                                                  t[2].as<float>()};

                    a->GetTransform().Rotation = {t[3].as<float>(), t[4].as<float>(),
                                                  t[5].as<float>()};

                    a->GetTransform().Scale = {t[6].as<float>(), t[7].as<float>(),
                                               t[8].as<float>()};
                }

                ComponentSerializer componentSerializer(a);
                componentSerializer.Deserialize(actor);

                // todo: On Parents and stuff
                scene->AddActor(a);
            }
        }
    }
} // namespace Core
