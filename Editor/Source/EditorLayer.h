#pragma once

#include "Core/Layer/Layer.h"

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnDetach();

        void OnUpdate();
        void OnImGuiRender();
    };
} // namespace Core
