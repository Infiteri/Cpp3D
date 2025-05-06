#pragma once

#include "Renderer/Camera/PerspectiveCamera.h"

namespace Core
{
    class EditorCamera
    {
    private:
        PerspectiveCamera *cam;

    public:
        float Speed = 1.0f;
        float FastSpeed = 3.0f;

        float Sensitivity = 0.05f;

    public:
        EditorCamera(PerspectiveCamera *target);
        ~EditorCamera() = default;

        void SetTarget(PerspectiveCamera *target);

        void Update();
    };
} // namespace Core
