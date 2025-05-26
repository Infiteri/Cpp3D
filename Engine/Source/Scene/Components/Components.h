#pragma once

#include "Base.h"

#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Object/Mesh.h"

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
} // namespace Core
