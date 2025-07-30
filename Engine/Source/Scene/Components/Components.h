#pragma once

#include "Base.h"

#include "Physics/Body/Config.h"
#include "Physics/Body/RigidBody.h"
#include "Physics/Body/StaticBody.h"
#include "Physics/Collider/Collider.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Object/Mesh.h"
#include <memory>
#include <string>

namespace Core
{
    class Actor;
    class Component
    {
    protected:
        Actor *owner;
        friend class Actor;

    public:
        Component() {};
        virtual ~Component() {};

        inline Actor *GetOwner() { return owner; };

        virtual void Start() {};
        virtual void Render() {};
        virtual void Update() {};
        virtual void Stop() {};

        virtual void From(Component *comp) {}
    };

    class CE_API MeshComponent : public Component
    {
    private:
        Mesh *mesh;

    public:
        MeshComponent();
        ~MeshComponent();

        void Render();

        void From(MeshComponent *mesh);

        inline Mesh *GetMesh() { return mesh; };
    };

    class CE_API PointLightComponent : public Component
    {
    public:
        PointLight Light;
        void Render();

        void From(PointLightComponent *p);
    };

    class CE_API SpotLightComponent : public Component
    {
    public:
        SpotLight Light;

        void Render();

        void From(SpotLightComponent *p);
    };

    class CE_API PerspectiveCameraComponent : public Component
    {
    public:
        PerspectiveCamera Camera;
        float FOV = 90.0f;
        float Near = 0.01f;
        float Far = 1000.0f;
        bool IsPrimary = true;

        void Start();

        void Render();

        void UpdateCamera();

        void From(PerspectiveCameraComponent *o);
    };

    class CE_API ScriptComponent : public Component
    {
    public:
        std::string ClassName;

        void Start();

        void From(ScriptComponent *other);
    };

    class CE_API RigidBodyComponent : public Component
    {
    private:
        RigidBody *body;

        friend class Scene;

    public:
        RigidBodyConfig Config;

        RigidBodyComponent();

        void From(RigidBodyComponent *other);

        inline RigidBody *GetBody() { return body; };
    };

    class CE_API StaticBodyComponent : public Component
    {
    private:
        StaticBody *body;

        friend class Scene;

    public:
        StaticBodyConfig Config;

        StaticBodyComponent();

        void From(StaticBodyComponent *other);

        inline StaticBody *GetBody() { return body; };
    };

    class CE_API ColliderComponent : public Component
    {
    public:
        ColliderConfig Config;

        void From(ColliderComponent *other);
    };
} // namespace Core
