#include "Input.h"
#include "Core/Engine.h"
#include "Core/Window/Window.h"
#include "GLFW/glfw3.h"

namespace Core
{
    static Input::State state;

    void Input::Init()
    {
        for (auto &pair : state.Keyboard.Current)
            pair.second = false;

        for (auto &pair : state.Mouse.Current)
            pair.second = false;
    }

    void Input::Shutdown() {}

    bool Input::GetKey(Keys key) { return state.Keyboard.Current[key]; }

    bool Input::GetKeyJustNow(Keys key) { return state.Keyboard.Just[key]; }

    bool Input::GetButton(Buttons button) { return state.Mouse.Current[button]; }

    void Input::SetMouseMode(MouseModes mode)
    {
        if (state.Mouse.CurrentMode == mode)
            return;

        state.Mouse.CurrentMode = mode;
        Window *window = Engine::GetWindow();
        CE_VERIFY(window);

        int glfwMode = 0;
        switch (mode)
        {
        case MouseModes::Visible:
            glfwMode = GLFW_CURSOR_NORMAL;
            break;

        case MouseModes::Hidden:
            glfwMode = GLFW_CURSOR_HIDDEN;
            break;

        case MouseModes::Locked:
            glfwMode = GLFW_CURSOR_DISABLED;
            break;
        }

        glfwSetInputMode(window->GetHandle(), GLFW_CURSOR, glfwMode);
    }

    MouseModes Input::GetMouseMode() { return state.Mouse.CurrentMode; }

    Vector2 Input::GetMousePosition() { return state.Mouse.Position; }

    int Input::GetMouseX() { return state.Mouse.Position.x; }

    int Input::GetMouseY() { return state.Mouse.Position.y; }

    Vector2 Input::GetMouseDelta() { return state.Mouse.Delta; }

    int Input::GetMouseDeltaX() { return state.Mouse.Delta.x; }

    int Input::GetMouseDeltaY() { return state.Mouse.Delta.y; }

    void InputUpdateKey(Keys key, bool p)
    {
        state.Keyboard.Last[key] = state.Keyboard.Current[key];
        state.Keyboard.Current[key] = p;
        state.Keyboard.Just[key] = (p && !state.Keyboard.Last[key]);
    }

    void InputUpdateMouseButton(Buttons button, bool p) { state.Mouse.Current[button] = p; }

    void InputUpdateMousePosition(int x, int y)
    {
        state.Mouse.Delta = {x - state.Mouse.Last.x, y - state.Mouse.Last.y};
        state.Mouse.Position = {(float)x, (float)y};
        state.Mouse.Last = {(float)x, (float)y};
    }
} // namespace Core
