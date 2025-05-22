#pragma once

#include "Core/Event/Event.h"
#include "Core/Layer/Layer.h"
#include "EditorCamera.h"
#include "EditorDock.h"
#include "EditorKeybind.h"
#include "EditorSettings.h"
#include "ImGuizmo.h"
#include "Panel/Panel.h"
#include "imgui.h"

// note: Kinda eh!
#define CE_LOG_NO_ACTIVE_SCENE()                                                                   \
    CE_LOG("CE_EDITOR", Error, "World has no active scene, cannot perform action")

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        struct State
        {
            PanelSystem Panels;

            EditorDock Dock;

            EditorKeybind Keybind;

            std::string ActiveScenePath = "";

            ImVec2 ViewportSize;

            EditorSettings Settings;

            EditorCamera Camera{nullptr};

            bool IsViewportFocused = false;

            ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::TRANSLATE;
        };

    public:
        EditorLayer() {};
        ~EditorLayer() {};

        void OnAttach();
        void OnDetach();

        void SerializeSettings();
        void UpdateWithSettings();

        void OnEvent(Event *event);

        void OnUpdate();
        void OnImGuiRender();

        // -- Dockspace --
        void DockspaceRenderSceneViewport();
        // ---------------

        // -- Scene --
        void SceneNew();
        void SceneOpen();
        void SceneOpen(const std::string &name);
        void SceneSave();
        void SceneSaveAs();
        // -----------

        // -- UI --
        void UI_TopMenuBar();
        // --------

        static EditorLayer *GetInstance();
    };
} // namespace Core
