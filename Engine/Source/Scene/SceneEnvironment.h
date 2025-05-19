#pragma once

#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Object/Sky.h"

namespace Core
{
    struct SceneEnvironment
    {
        DirectionalLight Light;
        Sky Sky;

        void From(SceneEnvironment &env);
    };
} // namespace Core
