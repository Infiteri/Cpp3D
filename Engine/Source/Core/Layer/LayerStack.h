#pragma once

#include "Base.h"
#include "Core/Layer/Layer.h"

namespace Core
{
    class CE_API LayerStack
    {
    public:
        LayerStack() {};
        virtual ~LayerStack() {};

        static void Init();
        static void Shutdown();

        static void OnUpdate();
        static void OnImGuiRender();

        static void PushLayer(Layer *layer);
        static void PopLayer(Layer *layer);
    };
} // namespace Core
