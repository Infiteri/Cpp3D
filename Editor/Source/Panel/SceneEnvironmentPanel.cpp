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
                const int ammount = 2;
                const char *selections[] = {"Color", "Skybox"};
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

                            case SkyMode::Skybox:
                                std::string src =
                                    Platform::OpenFileDialog(".ce_cubemap \0*.ce_cubemap\0");
                                if (!src.empty())
                                    sky->SetSkyboxMode(src);

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

            case Core::SkyMode::Skybox:
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
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }
} // namespace Core
