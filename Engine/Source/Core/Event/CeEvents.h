#pragma once

// note: All the engine known events should be here
#include "Core/Input.h"
namespace Core
{
    struct EventMouseButton
    {
        Buttons Button;
    };

    struct EventMouseMove
    {
        int X, Y;
    };

    struct EventKeyboardButton
    {
        Keys Key;
    };

    struct EventResize
    {
        int Width, Height;
    };
} // namespace Core
