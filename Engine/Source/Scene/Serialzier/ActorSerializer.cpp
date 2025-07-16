#include "ActorSerializer.h"

#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Scene/Serialzier/ComponentSerializer.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    ActorSerializer::ActorSerializer(Actor *target) { actor = target; }

    void ActorSerializer::Serialize(const std::string &name)
    {
        if (name.empty())
        {
            CE_LOG("CE_SCENE", Error, "Cannot serialize actor, name is empty");
            return;
        }

        if (!actor)
        {
            CE_LOG("CE_SCENE", Error, "Cannot serialize actor, actor is null");
            return;
        }

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Actors" << YAML::Value << YAML::BeginSeq;

        Serialize(out);

        out << YAML::EndSeq;
        out << YAML::EndMap;

        _SaveEmitter(out, name);
    }

    void ActorSerializer::Serialize(YAML::Emitter &out)
    {
        if (!actor)
        {
            CE_LOG("CE_SCENE", Error, "Cannot serialize actor, actor is null");
            return;
        }

        CE_DEBUG("SERIALIZE");

        out << YAML::BeginMap;

        auto a = actor;

        // General information
        CE_SERIALIZE_FIELD("Name", a->GetName().c_str());
        CE_SERIALIZE_FIELD("UUID", a->GetID().Get());

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

        CE_SERIALIZE_FIELD("Children", YAML::BeginSeq);
        auto target = actor;
        for (auto child : actor->GetChildren())
        {
            actor = child;
            Serialize(out);
        }

        actor = target;

        out << YAML::EndSeq;
        out << YAML::EndMap;
    }

    void ActorSerializer::Deserialize(const std::string &name)
    {
        if (name.empty())
        {
            CE_LOG("CE_SCENE", Error, "Empty file path when deserializing actor '%s'",
                   name.c_str());
            return;
        }

        YAML::Node data = _LoadEmitterData(name);

        if (!data || !data["Actors"])
        {
            CE_LOG("CE_SCENE", Error, "Unable to get actor data for file '%s'", name.c_str());
            return;
        }

        Deserialize(data["Actors"]);
    }

    void ActorSerializer::_DeserializeActor(YAML::Node actor, Actor *a)
    {
        a->SetName(actor["Name"].as<std::string>());
        a->id = actor["UUID"].as<u64>();

        // note: Transform if the format is different change here as well
        {
            auto t = actor["Transform"];
            a->GetTransform().Position = {t[0].as<float>(), t[1].as<float>(), t[2].as<float>()};

            a->GetTransform().Rotation = {t[3].as<float>(), t[4].as<float>(), t[5].as<float>()};

            a->GetTransform().Scale = {t[6].as<float>(), t[7].as<float>(), t[8].as<float>()};
        }

        ComponentSerializer componentSerializer(a);
        componentSerializer.Deserialize(actor);

        for (auto child : actor["Children"])
        {
            Actor *c = new Actor();
            _DeserializeActor(child, c);
            a->AddChild(c);
        }
    }

    void ActorSerializer::Deserialize(YAML::Node actor) { _DeserializeActor(actor, this->actor); }
} // namespace Core
