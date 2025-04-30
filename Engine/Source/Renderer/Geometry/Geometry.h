#pragma once

#include "Base.h"
#include "Math/Vector.h"
#include <vector>

namespace Core
{
    struct Vertex3D
    {
        Vector3 Position;
        Vector2 UV;
    };

    class Geometry
    {
    public:
        enum Type
        {
            None = 0,
            Box
        };

    protected:
        Type type;

    public:
        std::vector<Vertex3D> Vertices;
        std::vector<u32> Indices;

        Geometry() {};
        ~Geometry() {};

        inline Type GetType() { return type; };
    };

    class CE_API BoxGeometry : public Geometry
    {
    public:
        float Width, Height, Depth;
        BoxGeometry(float w, float h, float d);
    };
} // namespace Core
