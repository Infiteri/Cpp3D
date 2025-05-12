#pragma once

#include "Base.h"
#include "Core/Event/Event.h"

namespace Core
{
    class CE_API Layer
    {
    public:
        Layer() {};
        virtual ~Layer() {};

        virtual void OnEvent(Event *event) {};

        virtual void OnAttach() {};
        virtual void OnUpdate() {};
        virtual void OnImGuiRender() {};
        virtual void OnDetach() {};
    };
} // namespace Core
