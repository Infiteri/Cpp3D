#pragma once

#include "Base.h"

#include "Matrix.h"
#include "Vector.h"

namespace Core
{
    class CE_API Quaternion
    {
    public:
        union
        {
            struct
            {
                float x, y, z, w;
            };

            float data[4];
        };

        Quaternion(float x = 0, float y = 0, float z = 0, float w = 1);
        Quaternion(const Vector3 &axis, float angle);
        Quaternion(const Quaternion &other);
        ~Quaternion() = default;

        Quaternion operator*(const Quaternion &q)
        {
            float W = (q.w * w) - (q.x * x) - (q.y * y) - (q.z * z);
            float X = (q.w * x) + (q.x * w) + (q.y * z) - (q.z * y);
            float Y = (q.w * y) - (q.x * z) - (q.y * w) - (q.z * x);
            float Z = (q.w * z) - (q.x * y) - (q.y * x) - (q.z * w);
            Quaternion ret;
            ret.w = W;
            ret.x = X;
            ret.y = Y;
            ret.z = Z;
            return ret;
        }

        void Normalize();

        void Set(float x, float y, float z, float w);
        void Set(const Quaternion &other);

        Matrix4 GetMatrix();
    };

} // namespace Core
