#include "Color.h"

namespace Core
{
    Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    Color Color::Normalized() const { return {r / 255, g / 255, b / 255, a / 255}; }
} // namespace Core
