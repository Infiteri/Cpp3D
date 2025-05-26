#include "SceneEnvironment.h"

namespace Core
{

    void SceneEnvironment::From(SceneEnvironment &env)
    {
        Light.Color = env.Light.Color;
        Light.Intensity = env.Light.Intensity;
        Light.Direction = env.Light.Direction;

        Sky.From(env.Sky);
    }
} // namespace Core
