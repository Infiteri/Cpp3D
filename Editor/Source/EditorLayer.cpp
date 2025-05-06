#include "EditorLayer.h"
#include "Core/Engine.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Renderer.h"
#include "Scene/Components/Components.h"
#include "Scene/Scene.h"
#include "Scene/World.h"

#include <imgui.h>

namespace Core
{
    static EditorLayer::State state;

    void EditorLayer::OnAttach()
    {
        state.Camera = CameraSystem::GetActivePerspective();
        state.Camera.Sensitivity = 0.005;

        Scene *scene = World::Create("Test");
        World::Activate("Test");

        auto test1 = scene->CreateActor("TTV");
        test1->AddComponent<MeshComponent>();
        test1->GetTransform().Position = {3, 3, 0};

        auto test2 = test1->CreateChild("TTV2");
        test2->GetTransform().Position = {3, 0, 0};
        test2->AddComponent<MeshComponent>();
    }

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnUpdate()
    {
        if (state.IsViewportFocused)
            state.Camera.Update();

        if (Input::GetButton(Buttons::Right))
        {
            Vector2 pos = Input::GetMousePosition() +
                          Vector2(Engine::GetWindow()->GetX(), Engine::GetWindow()->GetY());

            ImVec2 LT = state.Dockspace.ViewportLeftTop;
            ImVec2 RB = state.Dockspace.ViewportRightBottom;

            if (pos.x > LT.x && pos.y > LT.y && pos.x < LT.x + RB.x && pos.y < LT.y + RB.y)
                state.IsViewportFocused = true;
        }
        else
            state.IsViewportFocused = false;
    }

    void EditorLayer::OnImGuiRender()
    {
        DockspaceBegin();
        state.Panels.RenderImGui();
        DockspaceRenderSceneViewport();
        ImGui::Begin("ImGui Style Editor");

        ImVec4 *colors = ImGui::GetStyle().Colors;
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char *name = ImGui::GetStyleColorName(i);
            ImGui::ColorEdit4(name, (float *)&colors[i]);
        }

        ImGui::End();
        DockspaceEnd();
    }

    // note: dockspace
    void EditorLayer::DockspaceBegin()
    {
        if (state.Dockspace.Fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            state.Dockspace.WindowFlags |= ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove;
            state.Dockspace.WindowFlags |=
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (state.Dockspace.Flags & ImGuiDockNodeFlags_PassthruCentralNode)
            state.Dockspace.WindowFlags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &state.Dockspace.Open, state.Dockspace.WindowFlags);
        ImGui::PopStyleVar();

        if (state.Dockspace.Fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 250.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspaceId = ImGui::GetID("Dock");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), state.Dockspace.Flags);
        }

        style.WindowMinSize.x = minWinSizeX;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::PopStyleVar();
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
        state.Dockspace.ViewportLeftTop = ImGui::GetWindowPos();
        state.Dockspace.ViewportRightBottom = ImGui::GetWindowSize();

        ImGui::Image((void *)(u64)(Renderer::GetSceneViewportPassID()), viewportSize, ImVec2{0, 1},
                     ImVec2{1, 0});

        ImGui::End();
    }

    void EditorLayer::DockspaceEnd() { ImGui::End(); }
} // namespace Core
