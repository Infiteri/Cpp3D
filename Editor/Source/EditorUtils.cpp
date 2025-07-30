#include "EditorUtils.h"
#include "Core/Data/CeData.h"
#include "Core/Logger.h"
#include "Core/Util/StringUtils.h"
#include "EditorTextureSystem.h"
#include "imgui.h"

#include <cstring>
#include <imgui_internal.h>

namespace Core
{
    namespace EditorUtils
    {
        static char stringBuffer[1024];

        bool ImGuiString(const char *label, std::string &str)
        {
            memset(stringBuffer, 0, 1024); // lol this used to be wrong cus funny
            memcpy(stringBuffer, str.data(), str.size());
            if (ImGui::InputText(label, stringBuffer, 1024))
            {
                str = stringBuffer;
                return true;
            }

            return false;
        }

        bool ImGuiColor(const char *label, Color &color)
        {
            float data[4] = {color.r / 255, color.g / 255, color.b / 255, color.a / 255};
            if (ImGui::ColorEdit4(label, data))
            {
                color.r = data[0] * 255;
                color.g = data[1] * 255;
                color.b = data[2] * 255;
                color.a = data[3] * 255;
                return true;
            }

            return false;
        }

        void ImGuiVector3(const char *label, Vector3 &vec)
        {
            float data[3] = {vec.x, vec.y, vec.z};
            if (ImGui::DragFloat3(label, data, 0.05))
            {
                vec.x = data[0];
                vec.y = data[1];
                vec.z = data[2];
            }
        }

        bool StringIsImageExtension(const std::string &ext)
        {
            if (ext == "png")
                return true;
            if (ext == "jpg")
                return true;
            if (ext == "jpeg")
                return true;
            if (ext == "ce_image")
                return true;
            return false;
        }

        void ImGuiVector3Styled(const char *label, Vector3 &vec, float defaultValue)
        {
            ImGui::PushID(label);

            ImGui::Columns(2);

            ImGui::SetColumnWidth(0, 50.0f);
            ImGui::Text(label);
            ImGui::NextColumn();
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

            float lineH = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = {lineH + 3.0f, lineH};
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});

            if (ImGui::Button("X", buttonSize))
                vec.x = defaultValue;

            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##x", &vec.x, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});

            if (ImGui::Button("Y", buttonSize))
                vec.y = defaultValue;

            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##y", &vec.y, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});

            if (ImGui::Button("Z", buttonSize))
                vec.z = defaultValue;

            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##z", &vec.z, 0.01f);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            ImGui::PopID();
        }

        void ImGuiTransformEdit(Transform &transform)
        {
            ImGuiVector3Styled("Position", transform.Position);
            ImGuiVector3Styled("Rotation", transform.Rotation);
            ImGuiVector3Styled("Scale", transform.Scale, 1.0);
        }

        void ImGuiCeDataSet(CeDataSet *set)
        {
            if (ImGui::Button("Add"))
            {
                set->Add("Data " + std::to_string(set->GetDataSet().size()));
                return;
            }
            int i = 0;
            for (auto &[name, data] : set->GetDataSet())
            {
                if (!data)
                    return;

                if (ImGui::TreeNodeEx((void *)(u64)(u32)i, 0, data->GetName().c_str()))
                {
                    {
                        std::string n = data->GetName();
                        if (ImGuiString("Name", n))
                        {
                            set->Rename(name, n);
                        }
                    }

                    const int maxSelections = 6;
                    const char *selections[maxSelections] = {"None", "Vec2",  "Vec3",
                                                             "Vec4", "Color", "Float"};
                    const char *current = selections[(int)data->GetType()];

                    {
                        if (ImGui::BeginCombo("Data Type", current))
                        {
                            for (int i = 0; i < maxSelections; i++)
                            {
                                bool isSelected = (current == selections[i]);

                                if (ImGui::Selectable(selections[i], isSelected))
                                {
                                    current = selections[i];
                                    data->_DestroyOnType();
                                    data->SetType((CeDataType)(i));
                                    data->_SetupDefaultOnType();
                                }

                                if (isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }

                            ImGui::EndCombo();
                        }
                    }

                    switch (data->GetType())
                    {
                    case CeDataType::Vector2:
                    {
                        auto v = data->As<Vector2>();
                        ImGui::DragFloat2("Data", &v->x, 0.01f);
                    }
                    break;
                    case CeDataType::Vector3:
                    {
                        auto v = data->As<Vector3>();
                        ImGui::DragFloat3("Data", &v->x, 0.01f);
                    }
                    break;
                    case CeDataType::Vector4:
                    {
                        auto v = data->As<Vector4>();
                        ImGui::DragFloat4("Data", &v->x, 0.01f);
                    }
                    break;
                    case CeDataType::Color:
                    {
                        auto v = data->As<Color>();
                        ImGuiColor("Data", *v);
                    }
                    break;
                    case CeDataType::Float:
                    {
                        auto v = data->As<float>();
                        ImGui::DragFloat("Data", v, 0.01f);
                    }
                    break;
                    }

                    if (ImGui::Button("Remove"))
                        set->Remove(name);

                    ImGui::TreePop();
                }

                i++;
            }
        }

        static bool MaterialConfiguratorRenderTexture(const char *label, std::string &path)
        {
            bool change = false;
            if (ImGui::TreeNode(label))
            {
                u32 texture = EditorTextureSystem::GetTexID(path);
                ImGui::Image((void *)(u64)texture, {100, 100});

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload =
                            ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
                    {
                        const char *str = (const char *)payload->Data;
                        std::string ext = StringUtils::GetFilenameExtension(str);

                        if (StringIsImageExtension(ext))
                        {
                            path = str;
                            change = true;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::TreePop();
            }

            return change;
        }

        void RenderMaterialConfig(Material::Configuration &Config)
        {
            EditorUtils::ImGuiColor("Color", Config.Color);

            // todo: Add metalic ao and roughness
            // to centuries of damn ive never been so yawn cant bealive im still standing cant
            // bealive life this long

            ImGui::DragFloat("Metallic", &Config.Metallic, 0.01f, 0.001f);
            ImGui::DragFloat("Roughness", &Config.Roughness, 0.01f, 0.001f);
            ImGui::DragFloat("AO", &Config.AO, 0.01f, 0.001f);

            MaterialConfiguratorRenderTexture("Color Texture", Config.ColorTexture);
            MaterialConfiguratorRenderTexture("Normal Texture", Config.NormalTexture);
        }
    } // namespace EditorUtils
} // namespace Core
