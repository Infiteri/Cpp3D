#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Core
{
    class CE_API Camera
    {
    protected:
        Matrix4 view;
        Matrix4 projection;
        Vector3 position;
        Vector3 rotation;

    public:
        Camera();
        virtual ~Camera();

        void SetPosition(const Vector3 &position);
        void SetRotation(const Vector3 &rot);

        void UpdateView();
        virtual void UpdateProjection() {};

        inline Matrix4 &GetView() { return view; };
        inline Matrix4 GetInvertedView() { return Matrix4::Invert(view); };
        inline Matrix4 &GetProjection() { return projection; };
        inline Vector3 &GetPosition() { return position; };
        inline Vector3 &GetRotation() { return rotation; };
    };
} // namespace Core
