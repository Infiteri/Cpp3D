#pragma once

#include "Panel.h"

namespace Core
{
    class SceneEnvironmentPanel : public Panel
    {
    public:
        SceneEnvironmentPanel() = default;
        ~SceneEnvironmentPanel() = default;

        void OnImGuiRender();
    };
} // namespace Core
