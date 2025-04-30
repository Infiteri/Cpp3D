#pragma once

#include "Camera.h"

namespace Core
{
    class CE_API PerspectiveCamera : public Camera
    {
    public:
        float FOV;
        float Aspect;
        float Near;
        float Far;

        PerspectiveCamera() = default;
        PerspectiveCamera(float fov, float aspect, float near = 0.01, float far = 1000);
        PerspectiveCamera(const PerspectiveCamera &other);
        ~PerspectiveCamera();

        void UpdateProjection();
        void UpdateProjection(float fov, float aspect, float near = 0.01, float far = 1000);
        void UpdateProjection(float aspect);
    };
} // namespace Core
