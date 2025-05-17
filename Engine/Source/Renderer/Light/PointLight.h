#pragma once

#include "Base.h"
#include "Light.h"

namespace Core
{
    class CE_API PointLight : public Light
    {
    public:
        Vector3 Position{0, 0, 0};
        Color Color{255, 255, 255, 255};
        Vector3 Specular{1, 1, 1};

        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;
        float Radius = 5;
        float Intensity = 1;

        PointLight();
        ~PointLight() = default;

        void Render(Shader *shader);
    };
} // namespace Core
