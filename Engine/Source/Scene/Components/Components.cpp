#include "Components.h"
#include "Base.h"

#include "Renderer/Shader/ShaderSystem.h"

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

        // todo: transforms
        mesh->Render(ShaderSystem::GetEngineResource("Object.glsl"));
    }

    void MeshComponent::From(Mesh *o)
    {
        CE_VERIFY(mesh && o);

        // todo:
    }

} // namespace Core
