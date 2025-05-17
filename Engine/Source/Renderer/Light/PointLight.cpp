#include "PointLight.h"
#include "Renderer/Light/Light.h"

namespace Core
{
    PointLight::PointLight() {}

    void PointLight::Render(Shader *shader)
    {
        CE_VERIFY(shader);

        //
        std::string fmt = "uPointLights[" + std::to_string(LightIDManager::PointGetID()) + "]";

        shader->Vec3(Position, (fmt + ".Position").c_str());
        shader->Vec4(Color, (fmt + ".Color").c_str());
        shader->Vec3(Specular, (fmt + ".Specular").c_str());
        shader->Float(Constant, (fmt + ".Constant").c_str());
        shader->Float(Linear, (fmt + ".Linear").c_str());
        shader->Float(Quadratic, (fmt + ".Quadratic").c_str());
        shader->Float(Radius, (fmt + ".Radius").c_str());
        shader->Float(Intensity, (fmt + ".Intensity").c_str());
    }
} // namespace Core
