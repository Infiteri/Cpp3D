#include "MaterialSystem.h"
#include "Core/Logger.h"
#include "Resource/MaterialLoader.h"

namespace Core
{
    static MaterialSystem::State state;

    void MaterialSystem::Init()
    {
        Material::Configuration defaultConfiguration;
        defaultConfiguration.Name = "Default Material Instance";
        defaultConfiguration.Color = {255, 255, 255, 255};
        state.DefaultMaterial = defaultConfiguration;
    }

    void MaterialSystem::Shutdown() {}

    Material *MaterialSystem::GetDefault() { return &state.DefaultMaterial; }

    void MaterialSystem::Remove(const std::string &name)
    {
        if (!Exists(name))
        {
            CE_LOG("CE_RENDER", Error, "Unable to remove material '%s' as it doesn't exist.");
            return;
        }

        state.Materials[name].Count--;
        if (state.Materials[name].Count <= 0)
        {
            state.Materials.erase(name);
        }
    }

    bool MaterialSystem::Exists(const std::string &name)
    {
        return state.Materials.find(name) != state.Materials.end();
    }

    Material *MaterialSystem::Add(const std::string &name)
    {
        if (Exists(name))
        {
            CE_LOG("CE_RENDER", Warn,
                   "Cannot Add material '%s' as it already exists, returning existing instance.",
                   name.c_str());
            state.Materials[name].Count++;
            return &state.Materials[name].Material;
        }

        Material::Configuration con;
        MaterialLoader loader;
        loader.Deserialize(name.c_str(), con);

        state.Materials[name] = {1, con};

        // todo: load material from filesystem when possible
        return &state.Materials[name].Material;
    }

    Material *MaterialSystem::Get(const std::string &name)
    {
        if (!Exists(name))
            return Add(name);

        state.Materials[name].Count++;
        return &state.Materials[name].Material;
    }
} // namespace Core
