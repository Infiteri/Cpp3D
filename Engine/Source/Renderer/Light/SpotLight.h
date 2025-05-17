#pragma once

#include "Base.h"
#include "Light.h"
#include "Math/Vector.h"

namespace Core
{
    class CE_API SpotLight : public Light
    {
    public:
        Vector3 Position{0.0f, 0.0f, 0.0f};
        Vector3 Direction{0.0f, -1.0f, 0.0f};
        Vector3 Diffuse{1.0f, 1.0f, 1.0f};
        Vector3 Specular{1.0f, 1.0f, 1.0f};

        Color Color{255.0f, 255.0f, 255.0f, 255.0f};

        float CutOff = 0.076f;
        float OuterCutOff = 0.953f;
        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        SpotLight() = default;
        ~SpotLight() = default;

        void Render(Shader *shader);
    };
} // namespace Core
