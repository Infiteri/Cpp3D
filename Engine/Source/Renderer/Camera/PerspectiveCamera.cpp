#include "PerspectiveCamera.h"

namespace Core
{
    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    {
        UpdateProjection(fov, aspect, near, far);
    }

    PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera &other)
    {
        UpdateProjection(other.FOV, other.Aspect, other.Near, other.Far);
    }

    PerspectiveCamera::~PerspectiveCamera() {}

    void PerspectiveCamera::UpdateProjection()
    {
        projection = Matrix4::Perspective(FOV, Aspect, Near, Far);
    }

    void PerspectiveCamera::UpdateProjection(float fov, float aspect, float near, float far)
    {
        FOV = fov;
        Near = near;
        Far = far;
        UpdateProjection(aspect);
    }

    void PerspectiveCamera::UpdateProjection(float aspect)
    {
        Aspect = aspect;
        UpdateProjection();
    }
} // namespace Core
