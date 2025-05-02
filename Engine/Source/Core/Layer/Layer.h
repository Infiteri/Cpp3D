#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Layer
    {
    public:
        Layer() {};
        virtual ~Layer() {};

        virtual void OnAttach() {};
        virtual void OnUpdate() {};
        virtual void OnImGuiRender() {};
        virtual void OnDetach() {};
    };
} // namespace Core
