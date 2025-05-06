#include "Components.h"
#include "Base.h"

#include "Core/Logger.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Scene/Actor.h"

namespace Core
{
    MeshComponent::MeshComponent() { mesh = new Mesh(); }

    MeshComponent::~MeshComponent()
    {
        CE_VERIFY(mesh);

        delete mesh;
    }

    void MeshComponent::Render()
    {
        CE_VERIFY(mesh)

        Matrix4 m = owner->GetGlobalMatrix();
        mesh->Render(ShaderSystem::GetEngineResource("Object.glsl"), m);
    }

    void MeshComponent::From(Mesh *o)
    {
        CE_VERIFY(mesh && o);

        // todo:
    }

} // namespace Core
