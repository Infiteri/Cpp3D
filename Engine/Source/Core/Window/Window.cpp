#include "Window.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Core
{
    static void CallbackViewport(GLFWwindow *handle, int w, int h) { Renderer::Viewport(w, h); }

    Window::Window(const Window::Configuration &config)
    {
        state = config;

        glfwInit();
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_DECORATED, state.Decorated);
        glfwWindowHint(GLFW_SAMPLES, 4);

        GLFWmonitor *targetMonitor = NULL;
        if (state.Size == FullScreen)
        {
            targetMonitor = glfwGetPrimaryMonitor();
            state.Width = glfwGetVideoMode(targetMonitor)->width;
            state.Height = glfwGetVideoMode(targetMonitor)->height;
        }

        // gen
        handle = glfwCreateWindow(state.Width, state.Height, state.Title, targetMonitor, NULL);
        glfwMakeContextCurrent(handle);
        glfwSetWindowPos(handle, state.X, state.Y);
        glfwShowWindow(handle);

        if (state.Size == Windowed)
            glfwMaximizeWindow(handle);

        glfwSwapInterval((state.VSync ? 1 : 0));
        // bind events
        glfwSetWindowSizeCallback(handle, CallbackViewport);

        // final setup
        glfwGetWindowPos(handle, &state.X, &state.Y);
        glfwGetWindowSize(handle, &state.Width, &state.Height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwGetWindowPos(handle, &state.X, &state.Y);
        glfwGetWindowSize(handle, &state.Width, &state.Height);

        // Input events
        {
            double x, y;
            glfwGetCursorPos(handle, &x, &y);
            InputUpdateMousePosition((int)x, (int)y);

            for (int key = 0; key < GLFW_KEY_LAST; key++)
                InputUpdateKey((Keys)key, glfwGetKey(handle, key) != 0);

            for (int button = 0; button < GLFW_MOUSE_BUTTON_LAST; button++)
                InputUpdateMouseButton((Buttons)button, glfwGetMouseButton(handle, button) != 0);
        }

        glfwPollEvents();
        glfwSwapBuffers(handle);
    }

    bool Window::ShouldRun() { return !glfwWindowShouldClose(handle); }

} // namespace Core
