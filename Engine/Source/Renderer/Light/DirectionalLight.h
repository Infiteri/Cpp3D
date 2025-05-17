#pragma once

#include "Base.h"
#include "Light.h"

namespace Core
{
    class CE_API DirectionalLight : public Light
    {
    public:
        Color Color{255, 255, 255, 255};
        Vector3 Direction{0, 0, -1};
        float Intensity = 1.0f;

        DirectionalLight();
        ~DirectionalLight() = default;

        void Render(Shader *shader);
    };
} // namespace Core
