#pragma once

#include "Renderer/Camera/PerspectiveCamera.h"

namespace Core
{
    class EditorCamera
    {
    private:
        PerspectiveCamera *cam;

    public:
        float Speed = 0.8f;
        float FastSpeed = 2.0f;
        float SlowSpeed = 0.5f;

        float Sensitivity = 0.005f;

    public:
        EditorCamera(PerspectiveCamera *target);
        ~EditorCamera() = default;

        void SetTarget(PerspectiveCamera *target);

        void Update();
    };
} // namespace Core
