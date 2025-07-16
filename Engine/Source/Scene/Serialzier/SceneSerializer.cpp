#include "SceneSerializer.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Renderer/Object/Sky.h"
#include "Scene/Actor.h"
#include "Scene/SceneEnvironment.h"
#include "Scene/Serialzier/ActorSerializer.h"
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
        {
            _SerializeActor(actor, emitter);
        }

        emitter << YAML::EndMap;

        _SaveEmitter(emitter, filepath);
    }

    void SceneSerializer::_SerializeActor(Actor *a, YAML::Emitter &out)
    {
        // note: Parented actors get handled in ActorSerializer
        if (a->GetParent() == nullptr)
        {
            ActorSerializer ser(a);
            ser.Serialize(out);
        }
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
                ActorSerializer ser(a);
                ser.Deserialize(actor);
                scene->AddActor(a);
            }
        }
        else
        {
            CE_LOG("CE_SCENE", Warn, "No actor node found");
        }
    }
} // namespace Core
