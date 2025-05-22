#pragma once

#include "Base.h"
#include "Vector.h"

namespace Core
{
    class CE_API Matrix4
    {
    public:
        union
        {
            float data[16];
            float m[4][4];
        };

        Matrix4();
        Matrix4(float *data);
        Matrix4(const Matrix4 &other);
        ~Matrix4() = default;

        Matrix4 operator*(const Matrix4 &o) { return Matrix4::Multiply(*this, o); }

        float &operator[](int index) { return data[index]; }

        void From(const Matrix4 &other);
        void From(const Matrix4 *other);

        // -- Static functions --
        static Matrix4 Identity();
        static Matrix4 Empty();
        static Matrix4 Invert(const Matrix4 &a);

        static Matrix4 Multiply(const Matrix4 &a, const Matrix4 &b);

        static Matrix4 Perspective(float fov, float aspect, float near, float far);

        static Matrix4 Translate(const Vector3 &vec);

        static Matrix4 RotateX(float x);
        static Matrix4 RotateY(float x);
        static Matrix4 RotateZ(float x);

        static Matrix4 RotateXYZ(const Vector3 &vec);
        static Matrix4 RotateZYX(const Vector3 &vec);

        static Matrix4 Scale(const Vector3 &vec);

        static Vector3 Forward(const Matrix4 &mat);
        static Vector3 Right(const Matrix4 &mat);
    };
} // namespace Core
