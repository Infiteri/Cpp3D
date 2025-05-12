#pragma once

#include "Core/Event/Event.h"
#include "Core/Layer/Layer.h"
#include "Editor/Source/Panel/Panel.h"
#include "EditorCamera.h"
#include "imgui.h"

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

            // todo: make editor camera
            EditorCamera Camera{nullptr};

            bool IsViewportFocused = false;
        };

    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnDetach();

        void OnEvent(Event *event);

        void OnUpdate();
        void OnImGuiRender();

        // -- Dockspace --
        void DockspaceBegin();
        void DockspaceRenderSceneViewport();
        void DockspaceEnd();
        // ---------------
    };
} // namespace Core
