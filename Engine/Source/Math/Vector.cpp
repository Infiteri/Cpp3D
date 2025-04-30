#include "Vector.h"

namespace Core
{
    Vector2::Vector2() { x = y = 0; }

    Vector2::Vector2(const Vector2 &other)
    {
        x = other.x;
        y = other.y;
    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {}

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector3::Vector3() { x = y = z = 0; }

    Vector3::Vector3(const Vector3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector4::Vector4() { x = y = z = w = 0; }

    Vector4::Vector4(const Vector4 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    void Vector4::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
} // namespace Core
