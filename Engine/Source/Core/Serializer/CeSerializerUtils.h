#pragma once

#include "Base.h"
#include "Math/Vector.h"
#include "Renderer/Color.h"

namespace YAML
{
    class Emitter;
    class Node;
} // namespace YAML

namespace Core
{
    namespace SerializerUtils
    {
        CE_API void SerializeVector2(const Vector2 &vec, const char *field, YAML::Emitter &em);
        CE_API void SerializeVector3(const Vector3 &vec, const char *field, YAML::Emitter &em);
        CE_API void SerializeVector4(const Vector4 &vec, const char *field, YAML::Emitter &em);
        CE_API void SerializeColor(const Color &vec, const char *field, YAML::Emitter &em);

        CE_API Vector2 DeserializeVector2(YAML::Node node);
        CE_API Vector3 DeserializeVector3(YAML::Node node);
        CE_API Vector4 DeserializeVector4(YAML::Node node);
        CE_API Color DeserializeColor(YAML::Node node);
    } // namespace SerializerUtils
} // namespace Core
