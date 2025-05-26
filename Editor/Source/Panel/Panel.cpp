#include "Panel.h"
#include "Panel/ContentBrowserPanel.h"
#include "Panel/LoggerPanel.h"
#include "Panel/SceneEnvironmentPanel.h"
#include "SceneHierarchyPanel.h"

namespace Core
{
    PanelSystem::PanelSystem()
    {
        Panels.push_back(new SceneHierarchyPanel());
        Panels.push_back(new ContentBrowserPanel());
        Panels.push_back(new SceneEnvironmentPanel());
        Panels.push_back(new LoggerPanel());
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
