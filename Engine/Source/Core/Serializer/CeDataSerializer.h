#pragma once

#include "CeSerializer.h"
#include "Core/Data/CeDataSet.h"

namespace YAML
{
    class Emitter;
}

namespace Core
{
    class CeDataSerializer : public CeSerializer
    {
    private:
        CeDataSet *set;

    public:
        CeDataSerializer(CeDataSet *target);

        void Serialize(const std::string &path);
        void Serialize(YAML::Emitter &out);

        void Deserialize(const std::string &path);
        void Deserialize(YAML::Node out);
    };
} // namespace Core
