#include "Transform.h"

#include "Math/Math.h"
#include "Quaternion.h"

namespace Core
{

    Transform::Transform()
    {
        Position = {0, 0, 0};
        Rotation = {0, 0, 0};
        Scale = {1, 1, 1};
    }

    Transform::Transform(const Transform &other)
    {
        Position = other.Position;
        Rotation = other.Rotation;
        Scale = other.Scale;
    }

    Matrix4 Transform::GetMatrix()
    {
        Quaternion quatX{{1, 0, 0}, Rotation.x * CE_DEG_TO_RAD};
        Quaternion quatY{{0, 1, 0}, Rotation.y * CE_DEG_TO_RAD};
        Quaternion quatZ{{0, 0, 1}, Rotation.z * CE_DEG_TO_RAD};
        Quaternion quat = quatX * quatY * quatZ;

        Matrix4 translationMatrix = Matrix4::Translate(Position);
        Matrix4 scaleMatrix = Matrix4::Scale(Scale);
        Matrix4 rotationMatrix = quat.GetMatrix();

        return scaleMatrix * (translationMatrix * rotationMatrix);
    }
} // namespace Core
