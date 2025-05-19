#pragma once

#include "Core/Event/Event.h"
#include "Core/Layer/Layer.h"
#include "EditorCamera.h"
#include "EditorSettings.h"
#include "Panel/Panel.h"
#include "imgui.h"
#include <string>

namespace Core
{
    // note: So far hardcoded, might remove this later
    struct DockspaceSettings
    {
        bool Open = true;
        bool Fullscreen = true;
        ImGuiDockNodeFlags Flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImVec2 ViewportLeftTop;
        ImVec2 ViewportRightBottom;
    };

    class EditorLayer : public Layer
    {
    public:
        struct State
        {
            PanelSystem Panels;

            DockspaceSettings Dockspace;
            ImVec2 ViewportSize;

            EditorSettings Settings;

            EditorCamera Camera{nullptr};

            bool IsViewportFocused = false;
        };

    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnDetach();

        void SerializeSettings();

        void OnEvent(Event *event);

        void OnUpdate();
        void OnImGuiRender();

        // -- Dockspace --
        void DockspaceBegin();
        void DockspaceRenderSceneViewport();
        void DockspaceEnd();
        // ---------------

        // -- UI --
        void UI_TopMenuBar();
        // --------
    };
} // namespace Core
