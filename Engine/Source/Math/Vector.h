#pragma once

#include "Base.h"
#include <cmath>

namespace Core
{
    class CE_API Vector2
    {
    public:
        float x, y;

        Vector2();
        Vector2(const Vector2 &other);
        Vector2(float x, float y);
        ~Vector2() = default;

        Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }

        Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }

        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

        Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }

        bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }

        bool operator!=(const Vector2 &other) const { return !(*this == other); }

        void Set(float x, float y);
    };

    class CE_API Vector3
    {
    public:
        float x, y, z;

        Vector3();
        Vector3(const Vector3 &other);
        Vector3(float x, float y, float z);
        ~Vector3() = default;

        Vector3 operator+(const Vector3 &other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3 &other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        Vector3 operator*(const Vector3 &other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        bool operator==(const Vector3 &other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Vector3 &other) const { return !(*this == other); }

        void operator+=(const Vector3 &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        }

        void operator-=(const Vector3 &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }

        void Normalize()
        {
            float l = sqrt(x * x + y * y + z * z);
            if (l > 0.0f)
            {
                x /= l;
                y /= l;
                z /= l;
            }
        }

        void Set(float x, float y, float z);
    };

    class CE_API Vector4
    {
    public:
        float x, y, z, w;

        Vector4();
        Vector4(const Vector4 &other);
        Vector4(float x, float y, float z, float w);
        ~Vector4() = default;

        Vector4 operator+(const Vector4 &other) const
        {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4 operator-(const Vector4 &other) const
        {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4 operator*(float scalar) const
        {
            return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        Vector4 operator/(float scalar) const
        {
            return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
        }

        Vector4 operator*(const Vector4 &other) const
        {
            return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
        }

        bool operator==(const Vector4 &other) const
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        bool operator!=(const Vector4 &other) const { return !(*this == other); }

        void Set(float x, float y, float z, float w);
    };
} // namespace Core
