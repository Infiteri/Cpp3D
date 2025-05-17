#include "DirectionalLight.h"

namespace Core
{
    DirectionalLight::DirectionalLight() {}

    void DirectionalLight::Render(Shader *shader)
    {
        CE_VERIFY(shader);

        shader->Float(Intensity, "DirLight.Intensity");
        shader->Vec4(Color, "DirLight.Color");
        shader->Vec3(Direction, "DirLight.Direction");
    }
} // namespace Core
