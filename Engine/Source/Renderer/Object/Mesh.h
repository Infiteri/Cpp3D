#pragma once

#include "Base.h"
#include "Math/Vector.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Shader/Shader.h"

namespace Core
{
    class CE_API Mesh
    {
    private:
        VertexArray *geometryArray;
        Material *material;
        Geometry *geometry;

        // Some internal information regarding the type of the current material
        MaterialType materialType = MaterialType::Default;

        void DestroyGeometryArray();
        void BufferGeometryArray();

        void DestroyMaterial();
        void DestroyGeometry();

    public:
        Mesh();
        ~Mesh();

        void SetMaterialToDefault();
        void SetMaterial(const std::string &name);
        void SetMaterial(const Material::Configuration &config);

        void SetGeometry(Geometry *geometry);

        inline Material *GetMaterial() { return material; };
        inline MaterialType GetMaterialType() { return materialType; };

        void Render(Shader *shader, const Matrix4 &transform);
    };
} // namespace Core
