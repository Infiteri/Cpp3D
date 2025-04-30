#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Shader/Shader.h"

namespace Core
{
    class CE_API Mesh
    {
    private:
        // Some internal information regarding the type of the current material
        enum class MaterialType
        {
            None = 0,
            Default,
            File,
            Config
        };

        MaterialType materialType = MaterialType::Default;

    private:
        VertexArray *geometryArray;
        Material *material;

        void DestroyGeometryArray();
        void BufferGeometryArray();

        void DestroyMaterial();

    public:
        Mesh();
        ~Mesh();

        void SetMaterialToDefault();

        void SetMaterial(const std::string &name);
        void SetMaterial(const Material::Configuration &config);

        inline Material *GetMaterial() { return material; };

        void Render(Shader *shader);
    };
} // namespace Core
