#include "EditorLayer.h"
#include "Base.h"
#include "Core/Engine.h"
#include "Core/Event/CeEvents.h"
#include "Core/Event/Event.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Util/StringUtils.h"
#include "EditorSettings.h"
#include "EditorToast.H"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Project/ProjectSystem.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture/CubemapTexture.h"
#include "Resource/CubemapLoader.h"
#include "Resource/MaterialLoader.h"
#include "Scene/Scene.h"
#include "Scene/Serialzier/SceneSerializer.h"
#include "Scene/World.h"

#include "Platform/Platform.h"

#include "Panel/SceneHierarchyPanel.h"

#include "EditorTextureSystem.h"

#include <ImGuizmo.h>
#include <imgui.h>
#include <string>

#define CE_SETTINGS_PATH "EditorSettings.ce_settings"

namespace Core
{
    static EditorLayer::State state;
    static EditorLayer *Instance = nullptr;

    void EditorLayer::OnAttach()
    {
        Instance = this;

        CE_DEFINE_LOG_CATEGORY("CE_EDITOR", "Editor");

        CameraSystem::AddPerspectiveCamera(
            "EditorCamera", {90, Renderer::GetViewport().GetAspect(), 0.01f, 1000.0f});
        CameraSystem::ActivatePerspectiveCamera("EditorCamera");
        CameraSystem::GetActivePerspective()->SetPosition({0, 0, 0});

        state.Camera = CameraSystem::GetActivePerspective();

        state.Settings.RegisterThemeColors();

        // Load editor settings
        // todo: Better placement?
        {
            EditorSettingsSerializer ser(&state.Settings);
            ser.Deserialize(CE_SETTINGS_PATH);
            UpdateWithSettings();
        }

        SetupFonts();

        ProjectSystem::New();
        ProjectSystem::GetActiveProject()->GetState().StartScene = "Assets/CM2.ce_scene";

        // todo: Setup this with some kind of Project structure
        World::Create(ProjectSystem::GetActiveProject()->GetStartScene());
        World::Activate(ProjectSystem::GetActiveProject()->GetStartScene());
        SceneOpen(ProjectSystem::GetActiveProject()->GetStartScene());
        SceneStopRuntime();
    }

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnEvent(Event *event)
    {
        if (Input::GetButton(Buttons::Right))
            return;

        if (event->GetType() == EventType::KeyboardButton)
        {
            EventKeyboardButton *kb = (EventKeyboardButton *)event->GetData();
            if (kb->Type == EventKeyboardButton::Press)
                state.Keybind.OnKeyDown(kb->Key);
        }
    }

    std::string EditorLayer::GetAssetPath(const std::string &name)
    {
        return "EngineAssets/" + name;
    }

    void EditorLayer::SetupFonts()
    {
        auto &io = ImGui::GetIO();
        io.Fonts->Clear();
        auto &font = state.Settings.General;

        ImFont *mainFont =
            io.Fonts->AddFontFromFileTTF(font.MainFont.File.c_str(), font.MainFont.Size);
        io.FontDefault = mainFont;
        io.Fonts->AddFontFromFileTTF(font.ToastFont.File.c_str(), font.ToastFont.Size);
        io.Fonts->Build();
    }

    void EditorLayer::SerializeSettings()
    {
        CE_LOG("CE_EDITOR", Trace, "Serializing scene settings");
        EditorSettingsSerializer ser(&state.Settings);
        ser.Serialize(CE_SETTINGS_PATH);
    }

    void EditorLayer::UpdateWithSettings()
    {
        auto target = state.Camera.GetTarget();
        auto &cam = state.Settings.General.Camera;

        CE_VERIFY(target);

        target->FOV = cam.FOV;
        state.Camera.Speed = cam.NormalSpeed;
        state.Camera.FastSpeed = cam.FastSpeed;
        state.Camera.SlowSpeed = cam.SlowSpeed;
        state.Camera.Sensitivity = cam.Sensitivity;

        target->UpdateProjection();
    }

    void EditorLayer::OnUpdate()
    {
        switch (state.CurrentSceneState)
        {
        case SceneState::Stop:
            OnEditorRuntime();
            break;

        case SceneState::Start:
            OnSceneRuntime();
            break;
        }
    }

    void EditorLayer::OnSceneRuntime() { World::UpdateActive(); }

    void EditorLayer::OnEditorRuntime()
    {
        state.Keybind.Update();

        if (state.IsViewportFocused)
            state.Camera.Update();

        if (Input::GetButton(Buttons::Right))
        {
            Vector2 pos = Input::GetMousePosition() +
                          Vector2(Engine::GetWindow()->GetX(), Engine::GetWindow()->GetY());

            ImVec2 LT = state.Dock.ViewportLeftTop;
            ImVec2 RB = state.Dock.ViewportRightBottom;

            if (pos.x > LT.x && pos.y > LT.y && pos.x < LT.x + RB.x && pos.y < LT.y + RB.y)
                state.IsViewportFocused = true;
        }
        else
            state.IsViewportFocused = false;
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGuizmo::BeginFrame();

        ToastMessage::OnImGuiRender();
        state.Dock.Begin();
        state.Panels.RenderImGui();

        DockspaceRenderSceneViewport();

        UI_TopMenuBar();
        UI_TopBar();

        {
            bool save = false;
            state.Settings.Theme.Render(save);
            if (save)
                SerializeSettings();

            state.Settings.General.Render(save);
            if (save)
            {
                SerializeSettings();
                UpdateWithSettings();
            }

            state.Popup.OnImGuiRender();
        }

        state.Dock.End();
    }

    void EditorLayer::HandleSceneViewportDragDrop()
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *load = ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
            {
                const char *path = (const char *)load->Data;
                std::string ext = StringUtils::GetFilenameExtension(path);
                if (ext == "ce_scene")
                    SceneOpen(path);
                else if (ext == "ce_cubemap")
                {
                    auto &cubemap = state.Popup.Cubemap;
                    cubemap.Active = true;
                    cubemap.ConfigPath = path;
                    CubemapLoader loader;
                    CubemapConfiguration config;
                    loader.Deserialize(config, path);
                    cubemap.Config = config;

                    const char *strs[] = {config.Right.c_str(), config.Left.c_str(),
                                          config.Top.c_str(),   config.Bottom.c_str(),
                                          config.Front.c_str(), config.Back.c_str()};

                    for (int i = 0; i < 6; i++)
                    {
                        cubemap.Textures[i].Destroy();
                        cubemap.Textures[i].Load(strs[i]);
                    }
                }
                else if (ext == "ce_mat")
                {
                    auto &material = state.Popup.Material;
                    material.Active = true;
                    material.Path = path;

                    MaterialLoader loader;
                    loader.Deserialize(path, material.Config);
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    void EditorLayer::DockspaceRenderSceneViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar();

        // todo: Resizing
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (state.ViewportSize.x != viewportSize.x || state.ViewportSize.y != viewportSize.y)
        {
            Renderer::Viewport(viewportSize.x, viewportSize.y);
            state.ViewportSize = viewportSize;
        }

        state.Dock.ViewportLeftTop = ImGui::GetWindowPos();
        state.Dock.ViewportRightBottom = ImGui::GetWindowSize();

        ImGui::Image((void *)(u64)(Renderer::GetSceneViewportPassID()), viewportSize, ImVec2{0, 1},
                     ImVec2{1, 0});

        // todo: Make this a function
        HandleSceneViewportDragDrop();

        // note: Gizmo stuff
        {
            auto panel = (SceneHierarchyPanel *)state.Panels.Panels[0];
            auto selected = panel->GetSelectedActor();
            auto camera = CameraSystem::GetActivePerspective();
            if (selected && camera)
            {
                auto &transform = selected->GetTransform();
                float *data = selected->GetGlobalMatrix().data;
                float *delta = selected->GetParent()
                                   ? Matrix4::Invert(selected->GetParent()->GetLocalMatrix()).data
                                   : NULL;

                float tmp[16];
                ImGuizmo::RecomposeMatrixFromComponents(
                    &transform.Position.x, &transform.Rotation.x, &transform.Scale.x, tmp);

                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                                  ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
                ImGuizmo::Manipulate(camera->GetInvertedView().data, camera->GetProjection().data,
                                     state.CurrentGizmoOperation, ImGuizmo::WORLD, tmp, delta);

                if (Input::GetKey(Keys::T))
                    state.CurrentGizmoOperation = ImGuizmo::TRANSLATE;
                if (Input::GetKey(Keys::R))
                    state.CurrentGizmoOperation = ImGuizmo::ROTATE;
                if (Input::GetKey(Keys::E))
                    state.CurrentGizmoOperation = ImGuizmo::SCALE;

                if (ImGuizmo::IsUsing())
                {
                    Matrix4 matrix = data;
                    if (selected->GetParent())
                        matrix = Matrix4::Multiply(selected->GetLocalMatrix(),
                                                   delta ? delta : Matrix4::Identity());

                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    ImGuizmo::DecomposeMatrixToComponents(tmp, matrixTranslation, matrixRotation,
                                                          matrixScale);

                    transform.Position = {matrixTranslation[0], matrixTranslation[1],
                                          matrixTranslation[2]};
                    transform.Rotation = {matrixRotation[0], matrixRotation[1], matrixRotation[2]};
                    transform.Scale = {matrixScale[0], matrixScale[1], matrixScale[2]};
                }
            }
        }

        ImGui::End();
    }

    void EditorLayer::UI_TopMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::MenuItem("File"))
                ImGui::OpenPopup("FilePopup");

            if (ImGui::MenuItem("Editor"))
                ImGui::OpenPopup("EditorPopup");

            if (ImGui::MenuItem("Other"))
                ImGui::OpenPopup("OtherPopup");

            if (ImGui::BeginPopup("FilePopup"))
            {
                if (ImGui::MenuItem("New...", "Ctrl + N"))
                    SceneNew();

                if (ImGui::MenuItem("Open...", "Ctrl + O"))
                    SceneOpen();

                if (ImGui::MenuItem("Save...", "Ctrl + S"))
                    SceneSave();

                if (ImGui::MenuItem("Save As...", "Ctrl + Shift + N"))
                    SceneSaveAs();

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("EditorPopup"))
            {
                if (ImGui::MenuItem("Settings"))
                    state.Settings.General.Active = true;

                if (ImGui::MenuItem("Theme"))
                    state.Settings.Theme.Active = true;

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("OtherPopup"))
            {
                if (ImGui::MenuItem("Ce Image Convertor"))
                {
                    ToastMessage::Add("Opening Image Convertor");
                    state.Popup.Image.Active = true;
                }

                ImGui::EndPopup();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void EditorLayer::UI_TopBar()
    {
        ImGui::Begin("##TopBar", nullptr, ImGuiWindowFlags_NoDecoration);

        float size = 12;
        const ImVec2 vecSize = {size, size};

        switch (state.CurrentSceneState)
        {
        case SceneState::Start:
            if (ImGui::ImageButton((void *)(u64)EditorTextureSystem::GetTexID(
                                       GetAssetPath("Icons/StopButton.png")),
                                   vecSize))
            {
                SceneStopRuntime();
            }
            break;

        case SceneState::Stop:
            if (ImGui::ImageButton((void *)(u64)EditorTextureSystem::GetTexID(
                                       GetAssetPath("Icons/PlayButton.png")),
                                   vecSize))
            {
                SceneStartRuntime();
            }
            break;
        }

        ImGui::End();
    }

    void EditorLayer::SceneNew()
    {
        ToastMessage::Add("New Scene");

        // todo: Some kind of scene saving stuff
        std::string name = std::to_string(UUID().Get());
        World::Create(name);
        World::Activate(name);
        state.ActiveScenePath = "";
    }

    void EditorLayer::SceneOpen()
    {
        std::string path = Platform::OpenFileDialog("*.ce_scene \0*.ce_scene\0");
        if (!path.empty())
            SceneOpen(path);
    }

    void EditorLayer::SceneOpen(const std::string &name)
    {
        if (state.ActiveScenePath == name)
        {
            CE_LOG("CE_SCENE", Warn, "Scene active already");
            return;
        }

        if (World::Exists(name))
        {
            // todo: Verify no memory leaks
            World::DeactivateActive();
            World::Remove(name);
            World::Create(name);
            SceneSerializer serializer(World::Get(name));
            serializer.Deserialize(name);
            World::Activate(name);
        }
        else
        {
            World::Create(name);
            SceneSerializer serializer(World::Get(name));
            serializer.Deserialize(name);
            World::Activate(name);
        }

        ToastMessage::Add("Opened Scene: " + name);

        state.ActiveScenePath = name;
    }

    void EditorLayer::SceneSave()
    {
        if (state.ActiveScenePath.empty())
            SceneSaveAs();
        else
        {
            SceneSerializer serializer(World::GetActive());
            serializer.Serialize(state.ActiveScenePath);
        }

        ToastMessage::Add("Saved Scene: " + state.ActiveScenePath);
    }

    void EditorLayer::SceneSaveAs()
    {
        state.ActiveScenePath = Platform::SaveFileDialog("*.ce_scene \0*.ce_scene\0");
        if (!state.ActiveScenePath.empty())
            SceneSave();
    }

    void EditorLayer::SceneStartRuntime()
    {
        if (state.CurrentSceneState == SceneState::Start)
            return;

        state.CurrentSceneState = SceneState::Start;

        // note: 1. Copy active scene to editor scene
        // 2. Stop active scene
        // 3. Set new active scene
        // 4. Start new scene

        World::StopActive();
        state.EditorScene = Scene::From(World::GetActive());
        World::StartActive();
    }

    void EditorLayer::SceneStopRuntime()
    {
        CameraSystem::ActivatePerspectiveCamera("EditorCamera");

        if (state.CurrentSceneState == SceneState::Stop)
            return;

        state.CurrentSceneState = SceneState::Stop;

        if (!state.EditorScene)
            return;

        auto panel = (SceneHierarchyPanel *)state.Panels.Panels[0];
        UUID id = 0;
        if (panel->GetSelectedActor())
            id = panel->GetSelectedActor()->GetID();
        panel->DeselectActor();

        World::StopActive();
        World::Editor_CopyToActive(state.EditorScene);
        delete state.EditorScene;
        state.EditorScene = nullptr;

        if (id.Get() != 0)
            panel->SelectActor(World::GetActive()->GetActorInAllHierarchy(id));
    }

    EditorLayer::SceneState EditorLayer::GetSceneState() { return state.CurrentSceneState; }

    EditorLayer *EditorLayer::GetInstance() { return Instance; }

} // namespace Core
