#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Scene/Actor.h"
#include "Scene/Components/Components.h"

namespace YAML
{
    class Node;
}

#define CE_SER_DESER_PAIR(type)                                                                    \
    void _Serialize##type##Component(type##Component *mesh, int index, YAML::Emitter &out);        \
    void _Deserialize##type##Component(YAML::Node node)

namespace Core
{
    class ComponentSerializer : public CeSerializer
    {
    private:
        Actor *actor;

        // note: In order for this to work the notation is TypeComponent
        // for mesh: MeshComponent results in Mesh in this struct
        // BoxColliderComponent -> BoxCollider
        // (notice how 'Component' at the end is stripped)
        struct ComponentCount
        {
            int Mesh = 0;
            int PointLight = 0;
            int SpotLight = 0;
            int PerspectiveCamera = 0;
            int Script = 0;
            int RigidBody = 0;
            int StaticBody = 0;
            int Collider = 0;
        };
        ComponentCount count;

        void _SerialzieComponentCount(YAML::Emitter &out);

        CE_SER_DESER_PAIR(Mesh);

        CE_SER_DESER_PAIR(PointLight);

        CE_SER_DESER_PAIR(SpotLight);

        CE_SER_DESER_PAIR(PerspectiveCamera);

        CE_SER_DESER_PAIR(Script);

        CE_SER_DESER_PAIR(RigidBody);

        CE_SER_DESER_PAIR(StaticBody);

        CE_SER_DESER_PAIR(Collider);

    public:
        ComponentSerializer(Actor *target);
        ~ComponentSerializer() = default;

        // Comes by default and stuff, compiler gives a warning
        void Serialize(const std::string &path) {};
        void Deserialize(const std::string &path) {};

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node &out);
    };
} // namespace Core
