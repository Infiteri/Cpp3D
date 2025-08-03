#include "Components.h"
#include "Base.h"

#include "Core/Logger.h"
#include "Physics/Collider/Collider.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Scene/Actor.h"
#include "Script/ScriptEngine.h"

namespace Core
{

    void CeDataComponent::From(CeDataComponent *other)
    {
        CE_VERIFY(other);

        for (auto &[name, data] : other->Set.GetDataSet())
        {
            CeData *c = new CeData();
            c->From(data);
            Set.Add(c, name);
        }
    }

    MeshComponent::MeshComponent() { mesh = new Mesh(); }

    MeshComponent::~MeshComponent()
    {
        CE_VERIFY(mesh);

        delete mesh;
    }

    void MeshComponent::Render()
    {
        CE_VERIFY(mesh);

        Matrix4 m = owner->GetGlobalMatrix();
        mesh->Render(ShaderSystem::GetEngineResource("Object.glsl"), m);
    }

    void MeshComponent::From(MeshComponent *o)
    {
        CE_VERIFY(mesh && o);

        // todo:
        switch (o->mesh->GetMaterialType())
        {
        case MaterialType::Default:
            mesh->SetMaterialToDefault();
            break;

        case MaterialType::File:
            mesh->SetMaterial(o->mesh->GetMaterial()->GetName());
            break;

        case MaterialType::Config:
            mesh->SetMaterial(o->mesh->GetMaterial()->GetState());
            break;

        default:
            CE_LOG("CE_SCENE", Warn, "Unable to copy material, type unknown");
            mesh->SetMaterialToDefault();
            break;
        }

        auto geometry = o->mesh->GetGeometry();
        switch (geometry->GetType())
        {
        case Geometry::None:
        default:
            mesh->SetGeometry(new Geometry());
            break;

        case Geometry::Box:
        {
            auto *cast = (BoxGeometry *)geometry;
            mesh->SetGeometry(new BoxGeometry(cast->Width, cast->Height, cast->Depth));
        }
        break;
        }
    }

    void PointLightComponent::Render()
    {
        Light.Position = owner->GetTransform().Position;
        Light.Render(ShaderSystem::GetEngineResource("Object.glsl"));
    }

    void PointLightComponent::From(PointLightComponent *p)
    {
        CE_VERIFY(p);

        Light.Intensity = p->Light.Intensity;
        Light.Color = p->Light.Color;
        Light.Position = p->Light.Position;
        Light.Constant = p->Light.Constant;
        Light.Linear = p->Light.Linear;
        Light.Quadratic = p->Light.Quadratic;
        Light.Radius = p->Light.Radius;
        Light.Specular = p->Light.Specular;
    }

    void SpotLightComponent::Render()
    {
        Light.Position = owner->GetTransform().Position;
        Light.Render(ShaderSystem::GetEngineResource("Object.glsl"));
    }

    void SpotLightComponent::From(SpotLightComponent *p)
    {
        CE_VERIFY(p);

        Light.Position = p->Light.Position;
        Light.Direction = p->Light.Direction;
        Light.Diffuse = p->Light.Diffuse;
        Light.Specular = p->Light.Specular;
        Light.Color = p->Light.Color;
        Light.CutOff = p->Light.CutOff;
        Light.OuterCutOff = p->Light.OuterCutOff;
        Light.Constant = p->Light.Constant;
        Light.Linear = p->Light.Linear;
        Light.Quadratic = p->Light.Quadratic;
    }

    void PerspectiveCameraComponent::Start() { UpdateCamera(); }

    void PerspectiveCameraComponent::Render()
    {
        Camera.SetPosition(owner->GetTransform().Position);
        Camera.SetRotation(owner->GetTransform().Rotation);
        Camera.UpdateView();
    }

    void PerspectiveCameraComponent::UpdateCamera()
    {
        Camera.FOV = FOV;
        Camera.Near = Near;
        Camera.Far = Far;

        Camera.UpdateProjection(Renderer::GetViewport().GetAspect());
    }

    void PerspectiveCameraComponent::From(PerspectiveCameraComponent *o)
    {
        FOV = o->FOV;
        Near = o->Near;
        Far = o->Far;

        IsPrimary = o->IsPrimary;
    }

    void ScriptComponent::Start() {}

    void ScriptComponent::From(ScriptComponent *other) { ClassName = other->ClassName; }

    RigidBodyComponent::RigidBodyComponent()
    {
        Config.ColliderConfig.Type = ColliderType::Box;
        Config.ColliderConfig.BoxType.Width = 1;
        Config.ColliderConfig.BoxType.Height = 1;
        Config.ColliderConfig.BoxType.Depth = 1;
    }

    void RigidBodyComponent::From(RigidBodyComponent *other)
    {
        if (!other)
            return;

        Config = other->Config;
    }

    StaticBodyComponent::StaticBodyComponent()
    {
        Config.ColliderConfig.Type = ColliderType::Box;
        Config.ColliderConfig.BoxType.Width = 1;
        Config.ColliderConfig.BoxType.Height = 1;
        Config.ColliderConfig.BoxType.Depth = 1;
    }

    void StaticBodyComponent::From(StaticBodyComponent *other)
    {
        if (!other)
            return;

        Config = other->Config;
    }

    void ColliderComponent::From(ColliderComponent *other) { Config = other->Config; }
} // namespace Core
