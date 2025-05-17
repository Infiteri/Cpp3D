#include "CeSerializerUtils.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    namespace SerializerUtils
    {
        void SerializeVector3(const Vector3 &vec, const char *field, YAML::Emitter &em)
        {
            em << YAML::Key << field << YAML::Value;
            em << YAML::Flow;
            em << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        }

        void SerializeColor(const Color &vec, const char *field, YAML::Emitter &em)
        {

            em << YAML::Key << field << YAML::Value;
            em << YAML::Flow;
            em << YAML::BeginSeq << vec.r << vec.g << vec.b << vec.a << YAML::EndSeq;
        }

        Color DeserializeColor(YAML::Node node)
        {
            Color c;

            if (node[0])
                c.r = node[0].as<float>();

            if (node[1])
                c.g = node[1].as<float>();

            if (node[2])
                c.b = node[2].as<float>();

            if (node[3])
                c.a = node[3].as<float>();

            return c;
        }

        Vector3 DeserializeVector3(YAML::Node node)
        {
            Vector3 v;

            if (node[0])
                v.x = node[0].as<float>();

            if (node[1])
                v.y = node[1].as<float>();

            if (node[2])
                v.z = node[2].as<float>();

            return v;
        }
    } // namespace SerializerUtils
} // namespace Core
