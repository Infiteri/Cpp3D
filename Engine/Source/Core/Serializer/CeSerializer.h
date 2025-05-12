#pragma once

#include "Base.h"
#include <string>

/// Assumes the emitter is named 'out', use CE)CE_SERIALIZE_FIELD_TO to serialize to an emitter
/// named other than 'out'
#define CE_SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value
#define CE_SERIALIZE_FIELD_TO(out, name, value) out << YAML::Key << name << YAML::Value << value

namespace YAML
{
    class Emitter;
    class Node;
} // namespace YAML

namespace Core
{
    class CE_API CeSerializer
    {
    protected:
        void _SaveEmitter(YAML::Emitter &em, const std::string &filepath);
        YAML::Node _LoadEmitterData(const std::string &filepath);

    public:
        CeSerializer() {};
        virtual ~CeSerializer() {};

        virtual void Serialize(const std::string &filepath) {};
        virtual void Deserialize(const std::string &filepath) {};
    };
} // namespace Core
