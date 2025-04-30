#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Color
    {
    public:
        float r, g, b, a;

        Color(float r = 255, float g = 255, float b = 255, float a = 255);
        ~Color() = default;

        Color Normalized() const;
    };
} // namespace Core
