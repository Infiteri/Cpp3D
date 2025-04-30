#pragma once

#include "Core/Layer/Layer.h"

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnInit();
        void OnDestroy();

        void OnUpdate();
        void OnImGuiRender();
    };
} // namespace Core
