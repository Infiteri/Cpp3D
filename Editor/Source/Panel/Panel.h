#pragma once

#include <vector>
namespace Core
{
    class Panel
    {
    public:
        Panel() {};
        virtual ~Panel() {};

        virtual void OnImGuiRender() {};
    };

    class PanelSystem
    {
    public:
        std::vector<Panel *> Panels;

        PanelSystem();
        ~PanelSystem();

        void RenderImGui();
    };
} // namespace Core
