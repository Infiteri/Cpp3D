#include "EditorUtils.h"
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
    } // namespace EditorUtils
} // namespace Core
