#include "SceneSerializer.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Renderer/Object/Sky.h"
#include "Scene/Actor.h"
#include "Scene/SceneEnvironment.h"
#include "Scene/Serialzier/ComponentSerializer.h"
#include "yaml-cpp/emittermanip.h"
#include "yaml-cpp/node/node.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    SceneSerializer::SceneSerializer(Scene *target) : scene(target) {}

    static void SerializeEnvironment(SceneEnvironment *env, YAML::Emitter &out)
    {
        CE_VERIFY(env);

        CE_SERIALIZE_FIELD("Environment", YAML::BeginMap);

        CE_SERIALIZE_FIELD("DirectionalLight", YAML::BeginMap);
        SerializerUtils::SerializeColor(env->Light.Color, "Color", out);
        SerializerUtils::SerializeVector3(env->Light.Direction, "Direction", out);
        CE_SERIALIZE_FIELD("Intensity", env->Light.Intensity);
        out << YAML::EndMap;

        CE_SERIALIZE_FIELD("Sky", YAML::BeginMap);
        {
            auto sky = &env->Sky;
            CE_SERIALIZE_FIELD("Mode", (int)sky->GetMode());
            switch (sky->GetMode())
            {
            case SkyMode::Color:
                SerializerUtils::SerializeColor(sky->GetColor(), "Color", out);
                break;

            case SkyMode::Skybox:
                CE_SERIALIZE_FIELD("LoadPath", sky->GetCubemapLoadPath().c_str());
                break;

            default:
                CE_LOG("CE_SCENE", Warn, "Unknown sky type");
                break;
            }
        }
        out << YAML::EndMap;

        out << YAML::EndMap;
    }

    static void DeserializeEnvironment(SceneEnvironment *env, YAML::Node node)
    {
        CE_VERIFY(env);

        auto lightNode = node["DirectionalLight"];
        auto skyNode = node["Sky"];
        if (!lightNode)
        {
            CE_LOG("CE_SCENE", Warn, "Light node not found");
            return;
        }
        if (!skyNode)
        {
            CE_LOG("CE_SCENE", Warn, "Sky node not found");
            return;
        }

        {
            env->Light.Color = SerializerUtils::DeserializeColor(lightNode["Color"]);
            env->Light.Direction = SerializerUtils::DeserializeVector3(lightNode["Direction"]);
            env->Light.Intensity = lightNode["Intensity"].as<float>();
        }

        {
            SkyMode mode = (SkyMode)skyNode["Mode"].as<int>();
            switch (mode)
            {
            case SkyMode::Color:
            {
                Color color = SerializerUtils::DeserializeColor(skyNode["Color"]);
                env->Sky.SetColorMode(color);
            }
            break;

            case SkyMode::Skybox:
            {
                std::string lp = skyNode["LoadPath"].as<std::string>();
                env->Sky.SetSkyboxMode(lp);
            }
            break;

            default:
                CE_LOG("CE_SCENE", Error, "Unknown sky mode when deserializing");
                break;
            }
        }
    }

    void SceneSerializer::Serialize(const std::string &filepath)
    {
        CE_VERIFY(scene);

        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        CE_SERIALIZE_FIELD_TO(emitter, "Scene", YAML::BeginMap);

        SerializeEnvironment(scene->GetEnvironment(), emitter);

        CE_SERIALIZE_FIELD_TO(emitter, "Actors", YAML::BeginSeq);

        // serialize Actors
        ActorList actors = scene->GetActors();
        for (auto actor : actors)
            _SerializeActor(actor, emitter);

        emitter << YAML::EndMap;

        _SaveEmitter(emitter, filepath);
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
        if (!scene)
        {
            CE_LOG("CE_SCENE", Error,
                   "Cannot deserialize to a nullptr scene, please set a VALID scene target to "
                   "deserialize");
            return;
        }

        YAML::Node data = _LoadEmitterData(filepath);

        if (!data || !data["Scene"])
        {
            CE_LOG("CE_SCENE", Error, "Unable to get scene data for file '%s'", filepath.c_str());
            return;
        }

        if (!data["Scene"]["Environment"])
            CE_LOG("CE_SCENE", Error, "Scene Environment root node not found in file '%s'",
                   filepath.c_str());
        else
            DeserializeEnvironment(scene->GetEnvironment(), data["Scene"]["Environment"]);

        auto actors = data["Scene"]["Actors"];
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
                auto parent = (actor["Parent"] ? actor["Parent"].as<u64>() : 0);
                if (parent != 0)
                {
                    Actor *target = scene->GetActorInAllHierarchy(parent);
                    if (target)
                        target->AddChild(a);
                    else
                    {
                        CE_LOG("CE_SCENE", Error,
                               "Unable find actor target %u, adding to scene instead of parent",
                               parent);
                        scene->AddActor(a);
                    }
                }
                else
                {
                    scene->AddActor(a);
                }
            }
        }
        else
        {
            CE_LOG("CE_SCENE", Warn, "No actor node found");
        }
    }
} // namespace Core
