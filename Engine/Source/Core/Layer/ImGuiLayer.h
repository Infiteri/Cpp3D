#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class CE_ED_API ImGuiLayer
    {
    public:
        ImGuiLayer() {};
        ~ImGuiLayer() {};

        static void Init();
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static void SetDefaultTheme();

        static void SetFont(const std::string &fontFile, float fontSize);
    };
} // namespace Core
