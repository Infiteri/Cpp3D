#pragma once

#include "Base.h"
#include "Renderer/Material/Material.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class CE_API MaterialSystem
    {
    public:
        struct MaterialReference
        {
            int Count = 0;
            Material Material;

            MaterialReference() : Count(0) {};
            MaterialReference(const int count) : Count(count) {};
        };

        struct State
        {
            std::unordered_map<std::string, MaterialReference> Materials;
            Material DefaultMaterial;
        };

    public:
        MaterialSystem() {};
        ~MaterialSystem() {};

        static void Init();
        static void Shutdown();

        static Material *GetDefault();

        static void Remove(const std::string &name);

        static bool Exists(const std::string &name);

        static Material *Add(const std::string &name);
        static Material *Get(const std::string &name);
    };
} // namespace Core
