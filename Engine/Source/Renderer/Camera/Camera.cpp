#include "Camera.h"

namespace Core
{
    Camera::Camera() {}

    Camera::~Camera() {}

    void Camera::SetPosition(const Vector3 &position)
    {
        this->position = position;
        UpdateView();
    }

    void Camera::SetRotation(const Vector3 &rot)
    {
        rotation = rot;
        UpdateView();
    }

    void Camera::UpdateView()
    {
        view = Matrix4::Translate(position);
        view = view * Matrix4::RotateZYX(rotation);
    }
} // namespace Core
