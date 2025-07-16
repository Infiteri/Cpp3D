#pragma once

// note: All the engine known events should be here
#include "Core/Input.h"
namespace Core
{
    struct EventResize
    {
        int Width, Height;

        EventResize(int width, int height) : Width(width), Height(height) {}
    };

    struct EventMouseButton
    {
        Buttons Button;

        EventMouseButton(Buttons button) : Button(button) {}
    };

    struct EventMouseMove
    {
        int X, Y;

        EventMouseMove(int x, int y) : X(x), Y(y) {}
    };

    struct EventKeyboardButton
    {
        enum Type
        {
            Press,
            Repeat,
            Release
        };

        Keys Key;
        Type EventType = Press;

        EventKeyboardButton(Keys key, Type type = Press) : Key(key), EventType(type) {}
    };
} // namespace Core
