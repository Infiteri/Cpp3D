#include "Core/Logger.h"
#include "ProjectSerialzier.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    ProjectSerializer::ProjectSerializer(Project *target) { project = target; }

    void ProjectSerializer::Serialize(const std::string &name)
    {
        if (!project)
        {
            CE_LOG("CE_PROJECT", Error, "Cannot serialize null project");
            return;
        }

        if (name.empty())
        {
            CE_LOG("CE_PROJECT", Error, "Cannot serialize project, filepath is empty");
            return;
        }

        YAML::Emitter out;

        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Project", YAML::BeginMap);

        CE_SERIALIZE_FIELD("Name", project->GetState().Name);
        CE_SERIALIZE_FIELD("AssetPath", project->GetState().AssetPath);
        CE_SERIALIZE_FIELD("StartScene", project->GetState().StartScene);
        CE_SERIALIZE_FIELD("LibraryPath", project->GetState().LibraryPath);

        out << YAML::EndMap;
        out << YAML::EndMap;

        _SaveEmitter(out, name);
    }

    void ProjectSerializer::Deserialize(const std::string &name)
    {
        if (!project)
        {
            CE_LOG("CE_PROJECT", Error, "Cannot deserialize null project");
            return;
        }

        if (name.empty())
        {
            CE_LOG("CE_PROJECT", Error, "Cannot deserialize project, filepath is empty");
            return;
        }

        YAML::Node data = _LoadEmitterData(name);
        if (!data)
        {
            CE_LOG("CE_PROJECT", Error, "Cannot deserialize project, file has no data");
            return;
        }

        if (!data["Project"])
        {
            CE_LOG("CE_PROJECT", Error, "Cannot deserialize project, file has no 'Project' node");
            return;
        }

        data = data["Project"];

        project->GetState().Name = data["Name"].as<std::string>();
        project->GetState().AssetPath = data["AssetPath"].as<std::string>();
        project->GetState().StartScene = data["StartScene"].as<std::string>();
        project->GetState().LibraryPath = data["LibraryPath"].as<std::string>();
    }
} // namespace Core
