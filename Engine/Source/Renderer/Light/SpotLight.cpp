#include "SpotLight.h"

namespace Core
{
    void SpotLight::Render(Shader *shader)
    {
        CE_VERIFY(shader);

        std::string fmt = "uSpotLights[" + std::to_string(LightIDManager::SpotGetID()) + "]";

        shader->Vec3(Position, (fmt + ".Position").c_str());
        shader->Vec3(Diffuse, (fmt + ".Diffuse").c_str());
        shader->Vec3(Direction, (fmt + ".Direction").c_str());
        shader->Vec3(Specular, (fmt + ".Specular").c_str());

        shader->Vec4(Color, (fmt + ".Color").c_str());

        shader->Float(Constant, (fmt + ".Constant").c_str());
        shader->Float(Linear, (fmt + ".Linear").c_str());
        shader->Float(Quadratic, (fmt + ".Quadratic").c_str());
        shader->Float(CutOff, (fmt + ".CutOff").c_str());
        shader->Float(OuterCutOff, (fmt + ".OuterCutOff").c_str());
    }
} // namespace Core
