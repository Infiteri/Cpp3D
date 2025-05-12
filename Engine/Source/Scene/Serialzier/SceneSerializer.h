#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Scene/Scene.h"

namespace Core
{
    class CE_API SceneSerializer : public CeSerializer
    {
    private:
        Scene *scene;

        void _SerializeActor(Actor *a, class YAML::Emitter &out);

    public:
        SceneSerializer(Scene *target);

        void Serialize(const std::string &filepath);
        void Deserialize(const std::string &filepath);
    };
} // namespace Core
