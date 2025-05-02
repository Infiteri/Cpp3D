#pragma once

#include "Base.h"

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

        void From(Mesh *mesh);

        inline Mesh *GetMesh() { return mesh; };
    };
} // namespace Core
