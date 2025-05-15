#include "Panel.h"
#include "SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"

namespace Core
{
    PanelSystem::PanelSystem()
    {
        Panels.push_back(new SceneHierarchyPanel());
        Panels.push_back(new ContentBrowserPanel());
    }

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
