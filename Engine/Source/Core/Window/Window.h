#pragma once

#include "Base.h"

typedef struct GLFWwindow GLFWwindow;

namespace Core
{
    class Window
    {
    public:
        enum Size
        {
            Normal,
            Windowed,
            FullScreen,
        };

        struct Configuration
        {
            int X = 0;
            int Y = 0;
            int Width = 1280;
            int Height = 720;
            const char *Title = "Window";

            Size Size = Normal;
            bool VSync = true;
            bool Decorated = true;
        };

    private:
        Configuration state;
        GLFWwindow *handle;

    public:
        Window(const Configuration &config);
        ~Window();

        void Update();
        bool ShouldRun();

        inline int GetX() { return state.X; };
        inline int GetY() { return state.Y; };
        inline float GetWidth() { return state.Width; };
        inline float GetHeight() { return state.Height; };

        inline GLFWwindow *GetHandle() { return handle; };
    };

} // namespace Core
