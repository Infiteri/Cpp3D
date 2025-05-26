#pragma once

#include "Core/Event/Event.h"
#include "Core/Layer/Layer.h"
#include "EditorCamera.h"
#include "EditorDock.h"
#include "EditorKeybind.h"
#include "EditorPopup.h"
#include "EditorSettings.h"
#include "ImGuizmo.h"
#include "Panel/Panel.h"
#include "Scene/Scene.h"
#include "imgui.h"

// note: Kinda eh!
#define CE_LOG_NO_ACTIVE_SCENE()                                                                   \
    CE_LOG("CE_EDITOR", Error, "World has no active scene, cannot perform action")

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        enum class SceneState
        {
            Start,
            Stop
        };

        struct State
        {
            PanelSystem Panels;

            SceneState CurrentSceneState = SceneState::Stop;
            Scene *EditorScene = nullptr;

            EditorPopupSystem Popup;

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

        std::string GetAssetPath(const std::string &name);

        void SerializeSettings();
        void UpdateWithSettings();

        void SetupFonts();

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

        void SceneStartRuntime();
        void SceneStopRuntime();
        // -----------

        // -- UI --
        void UI_TopMenuBar();
        void UI_TopBar();
        // --------

        static EditorLayer *GetInstance();
    };
} // namespace Core
