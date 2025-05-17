#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Scene/Actor.h"
#include "Scene/Components/Components.h"

namespace YAML
{
    class Node;
}

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
        };
        ComponentCount count;

        void _SerialzieComponentCount(YAML::Emitter &out);

        void _SerializeMeshComponent(MeshComponent *mesh, int index, YAML::Emitter &out);
        void _DeserializeMeshComponent(YAML::Node node);

        void _SerializePointLightComponent(PointLightComponent *c, int index, YAML::Emitter &out);
        void _DeserializePointLightComponent(YAML::Node node);

        void _SerializeSpotLightComponent(SpotLightComponent *c, int index, YAML::Emitter &out);
        void _DeserializeSpotLightComponent(YAML::Node node);

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
