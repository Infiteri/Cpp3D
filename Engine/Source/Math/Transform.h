#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Core
{
    struct CE_API Transform
    {
        Vector3 Position{0, 0, 0};
        Vector3 Rotation{0, 0, 0};
        Vector3 Scale{1, 1, 1};

        Transform();
        Transform(const Transform &other);
        ~Transform() = default;

        Matrix4 GetMatrix();
    };
} // namespace Core
