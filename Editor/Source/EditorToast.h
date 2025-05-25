#pragma once

#include <imgui.h>
#include <string>
#include <vector>

namespace Core
{
    struct Toast
    {
        std::string Message;
        float Lifetime;
        float TotalLifetime;
    };

    class ToastMessage
    {
    public:
        struct State
        {
            std::vector<Toast> Toasts;
        };

    public:
        ToastMessage() = delete;
        ~ToastMessage() = delete;

        static void Add(const std::string &msg, float life = 1.0f);

        static void OnImGuiRender();
    };
} // namespace Core
