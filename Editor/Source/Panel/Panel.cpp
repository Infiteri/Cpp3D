#include "Panel.h"
#include "Editor/Source/Panel/SceneHierarchyPanel.h"

namespace Core
{
    PanelSystem::PanelSystem() { Panels.push_back(new SceneHierarchyPanel()); }

    PanelSystem::~PanelSystem()
    {
        for (auto p : Panels)
            delete p;
    }

    void PanelSystem::RenderImGui()
    {
        for (auto p : Panels)
            p->OnImGuiRender();
    }
} // namespace Core
