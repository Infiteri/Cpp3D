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
        CE_API void SerializeVector3(const Vector3 &vec, const char *field, YAML::Emitter &em);
        CE_API void SerializeColor(const Color &vec, const char *field, YAML::Emitter &em);

        CE_API Color DeserializeColor(YAML::Node node);
    } // namespace SerializerUtils
} // namespace Core
