#pragma once

#include "Math/Vector.h"
#include "Renderer/Color.h"
#include "Scene/Actor.h"
#include <imgui.h>
#include <string>
#include <typeinfo>

namespace Core
{
    namespace EditorUtils
    {
        bool ImGuiString(const char *label, std::string &str);
        bool ImGuiColor(const char *label, Color &color);
        void ImGuiVector3(const char *label, Vector3 &vec);
        void ImGuiVector3Styled(const char *label, Vector3 &vec, float defaultValue = 0.0f);
        void ImGuiTransformEdit(Transform &transform);

        template <typename T, typename UIFun>
        static void DrawComponentBaseUI(const std::string &name, T *component, int index, Actor *a,
                                        UIFun fun)
        {
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
            if (component)
            {

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});
                bool open =
                    ImGui::TreeNodeEx((void *)(typeid(T).hash_code() + index), flags, name.c_str());
                bool removeComp = false;

                ImGui::PopStyleVar();

                if (open)
                {
                    fun(component, a);
                    if (ImGui::Button("Remove"))
                    {
                        removeComp = true;
                    };
                    ImGui::TreePop();
                }

                if (removeComp)
                {
                    a->RemoveComponent<T>(index);
                }
            }
        };
    } // namespace EditorUtils
} // namespace Core
