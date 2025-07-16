#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Scene/Actor.h"
#include <string>

namespace Core
{
    class CE_API ActorSerializer : public CeSerializer
    {
    private:
        Actor *actor;

        void _DeserializeActor(YAML::Node actor, Actor *a);

    public:
        ActorSerializer(Actor *target);

        void Serialize(const std::string &name);
        void Serialize(YAML::Emitter &out);

        void Deserialize(const std::string &name);
        void Deserialize(YAML::Node node);
    };
} // namespace Core
