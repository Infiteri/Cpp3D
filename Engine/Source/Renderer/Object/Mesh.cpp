#include "Mesh.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Material/MaterialSystem.h"

#include <glad/glad.h>

namespace Core
{
    Mesh::Mesh() : geometryArray(nullptr)
    {
        geometry = new BoxGeometry(1, 1, 1);

        BufferGeometryArray();
        SetMaterialToDefault();
    }

    Mesh::~Mesh()
    {
        DestroyGeometryArray();
        DestroyMaterial();
        DestroyGeometry();
    }

    void Mesh::DestroyGeometryArray()
    {
        if (geometryArray)
            delete geometryArray;

        geometryArray = nullptr;
    }

    void Mesh::BufferGeometryArray()
    {
        CE_VERIFY(geometry);

        DestroyGeometryArray();
        geometryArray = new VertexArray();
        geometryArray->GenerateVertexBuffer(geometry->Vertices.data(),
                                            geometry->Vertices.size() * sizeof(Vertex3D));
        geometryArray->GetVertexBuffer()->AddLayout(0, 0, 3);
        geometryArray->GetVertexBuffer()->AddLayout(1, 3, 2);
        geometryArray->GenerateIndexBuffer(geometry->Indices.data(),
                                           geometry->Indices.size() * sizeof(u32));
    }

    void Mesh::DestroyGeometry()
    {
        if (geometry)
            delete geometry;

        geometry = nullptr;
    }

    void Mesh::DestroyMaterial()
    {
        CE_VERIFY(material);

        switch (materialType)
        {
        case MaterialType::Config:
            delete material;
            break;

        case MaterialType::File:
            MaterialSystem::Remove(material->GetName());
            break;

        default:
            break;
        }

        material = nullptr;
        materialType = MaterialType::None;
    }

    void Mesh::SetMaterialToDefault()
    {
        DestroyMaterial();

        material = MaterialSystem::GetDefault();
        materialType = MaterialType::Default;
    }

    void Mesh::SetMaterial(const std::string &name)
    {
        DestroyMaterial();
        material = MaterialSystem::Get(name);
        materialType = MaterialType::File;
    }

    void Mesh::SetMaterial(const Material::Configuration &config)
    {
        DestroyMaterial();
        material = new Material(config);
        materialType = MaterialType::Config;
    }

    void Mesh::SetGeometry(Geometry *geometry)
    {
        DestroyGeometry();

        this->geometry = geometry;
    }

    void Mesh::Render(Shader *shader, const Matrix4 &transform)
    {
        CE_VERIFY(material);

        if (!geometryArray)
            BufferGeometryArray();

        shader->Mat4(transform, "uTransform");

        material->Use(shader);

        geometryArray->Bind();
        geometryArray->GetVertexBuffer()->Bind();
        geometryArray->GetIndexBuffer()->Draw();
    }
} // namespace Core
