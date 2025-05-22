#pragma once

#include "Core/Input.h"
namespace Core
{
    class EditorKeybind
    {
    public:
        EditorKeybind() = default;
        ~EditorKeybind() = default;

        void Update();
        void OnKeyDown(Keys key);
    };
} // namespace Core
