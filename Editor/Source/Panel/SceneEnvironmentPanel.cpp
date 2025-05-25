#include "SceneEnvironmentPanel.h"
#include "Core/Util/StringUtils.h"
#include "EditorUtils.h"

#include "Renderer/Object/Sky.h"
#include "Scene/World.h"

#include "Platform/Platform.h"

#include <imgui.h>

namespace Core
{
    void SceneEnvironmentPanel::OnImGuiRender()
    {
        if (!World::GetActive())
        {
            ImGui::Begin("Scene Environment");
            ImGui::TextColored({1, 0, 0, 1}, "No active scene.");
            ImGui::End();
            return;
        }
        auto env = World::GetActive()->GetEnvironment();

        ImGui::Begin("Scene Environment");

        if (ImGui::TreeNode("Directional Light"))
        {
            auto light = &env->Light;
            EditorUtils::ImGuiColor("Color", light->Color);
            EditorUtils::ImGuiVector3("Direciton", light->Direction);
            ImGui::DragFloat("Intensity", &light->Intensity, 0.01, 0.01);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sky"))
        {
            auto sky = &env->Sky;

            {
                const int ammount = 3;
                const char *selections[] = {"Color", "Skybox", "Shader"};
                const char *current = selections[(int)sky->GetMode()];

                if (ImGui::BeginCombo("Type", current))
                {
                    for (int i = 0; i < ammount; i++)
                    {
                        bool isSelected = (current == selections[i]);

                        if (ImGui::Selectable(selections[i], isSelected))
                        {
                            current = selections[i];

                            switch ((SkyMode)i)
                            {
                            case SkyMode::Color:
                                sky->SetColorMode({});
                                break;

                            case SkyMode::Shader:
                            {
                                std::string src = Platform::OpenFileDialog(".glsl \0*.glsl\0");
                                if (!src.empty())
                                    sky->SetShaderMode(src);
                            }
                            break;

                            case SkyMode::Skybox:
                            {
                                std::string src =
                                    Platform::OpenFileDialog(".ce_cubemap \0*.ce_cubemap\0");
                                if (!src.empty())
                                    sky->SetSkyboxMode(src);
                            }
                            break;
                            }
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
            }
            switch (sky->GetMode())
            {
            case SkyMode::Color:
                EditorUtils::ImGuiColor("Color", sky->GetColor());
                break;

            case SkyMode::Skybox:
                if (ImGui::Button("Load"))
                {
                    std::string src = Platform::OpenFileDialog(".ce_cubemap \0*.ce_cubemap\0");

                    if (!src.empty())
                        sky->SetSkyboxMode(src);
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload =
                            ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
                    {
                        const char *path = (const char *)payload->Data;
                        std::string ext = StringUtils::GetFilenameExtension(path);

                        if (ext == "ce_cubemap")
                            sky->SetSkyboxMode(path);
                    }

                    ImGui::EndDragDropTarget();
                }

                break;

            case SkyMode::Shader:
                if (ImGui::Button("Load"))
                {
                    std::string src = Platform::OpenFileDialog(".glsl \0*.glsl\0");

                    if (!src.empty())
                        sky->SetShaderMode(src);
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload =
                            ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
                    {
                        const char *path = (const char *)payload->Data;
                        std::string ext = StringUtils::GetFilenameExtension(path);

                        if (ext == "glsl")
                            sky->SetShaderMode(path);
                    }

                    ImGui::EndDragDropTarget();
                }

                if (ImGui::TreeNode("Shader Data"))
                {
                    EditorUtils::ImGuiCeDataSet(sky->GetShaderDataSet());

                    ImGui::TreePop();
                }

                break;
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }
} // namespace Core
