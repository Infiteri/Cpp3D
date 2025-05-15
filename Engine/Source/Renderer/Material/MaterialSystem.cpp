#include "MaterialSystem.h"
#include "Core/Logger.h"
#include "Resource/MaterialLoader.h"

#define CE_DEBUG_MATERIAL_SYS 1

#if CE_DEBUG_MATERIAL_SYS == 1
#define CE_MAT_DBG(msg, ...) CE_LOG("CE_RENDER", Trace, msg, ##__VA_ARGS__)
#else
#define CE_MAT_DBG(msg, ...)
#endif

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

        state.Materials[name]->Count--;
        CE_MAT_DBG("Removing material '%s', new count = %i", name.c_str(),
                   state.Materials[name]->Count);
        if (state.Materials[name]->Count <= 0)
        {
            delete state.Materials[name];
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
            state.Materials[name]->Count++;
            return &state.Materials[name]->Material;
        }

        Material::Configuration con;
        MaterialLoader loader;
        loader.Deserialize(name.c_str(), con);
        con.Name = name;

        state.Materials[name] = new MaterialReference{1, con};

        CE_MAT_DBG("Added material '%s'", name.c_str());

        // todo: load material from filesystem when possible
        return &state.Materials[name]->Material;
    }

    Material *MaterialSystem::Get(const std::string &name)
    {
        if (!Exists(name))
            return Add(name);

        state.Materials[name]->Count++;

        CE_MAT_DBG("Getting material '%s', new count = %i", name.c_str(),
                   state.Materials[name]->Count);

        return &state.Materials[name]->Material;
    }
} // namespace Core
