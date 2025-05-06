#include "SceneHierarchyPanel.h"
#include "Base.h"
#include "Core/Logger.h"
#include "EditorUtils.h"
#include "Renderer/Material/Material.h"
#include "Scene/Components/Components.h"
#include "Scene/World.h"

#include <imgui.h>

#define CE_RENDER_COMP(label, type, func)                                                          \
    {                                                                                              \
        int index = -1;                                                                            \
        for (auto c : a->GetComponents<type>())                                                    \
        {                                                                                          \
            index++;                                                                               \
            ImGui::NewLine();                                                                      \
            EditorUtils::DrawComponentBaseUI(label, c, index, a, func);                            \
        }                                                                                          \
    }

namespace Core
{
    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy Panel");

        if (World::GetActive())
            for (auto actor : World::GetActive()->GetActors())
                RenderActor(actor);
        ImGui::End();

        ImGui::Begin("Components");

        if (selected)
            RenderActorComponents(selected);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a, Actor *parent, bool parentNodeOpen)
    {
        CE_VERIFY(a);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap |
                                   ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                   ImGuiTreeNodeFlags_OpenOnArrow;
        if (selected)
        {
            if (selected->GetID() == a->GetID())
                flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool pop = false;

        if (!parent || parentNodeOpen)
        {
            pop = ImGui::TreeNodeEx((void *)(u64)a->GetID(), flags, a->GetName().c_str());
            if (ImGui::IsItemClicked())
                selected = a;
        }

        // todo: drag and drop to pair children to parents
        // todo: delete actors

        for (auto child : a->GetChildren())
            RenderActor(child, a, pop);

        if (pop)
            ImGui::TreePop();
    }

    void RenderMeshUI(MeshComponent *m, Actor *a);

    void SceneHierarchyPanel::RenderActorComponents(Actor *a)
    {
        CE_VERIFY(a);

        // note: Name rendering
        std::string name = a->GetName();
        if (EditorUtils::ImGuiString("Name", name))
            a->SetName(name);

        ImGui::NewLine();

        // note: Transform rendering
        EditorUtils::ImGuiTransformEdit(a->GetTransform());

        CE_RENDER_COMP("Mesh Component", MeshComponent, RenderMeshUI);
    }

    void RenderMeshUI(MeshComponent *m, Actor *a)
    {
        CE_VERIFY(m && a);
        // note: material

        auto material = m->GetMesh()->GetMaterial();
        auto mesh = m->GetMesh();

        bool renderButtonMakeDefault = false;
        bool renderButtonMakeConfig = false;
        bool renderButtonMakeFile = false;

        {
            switch (mesh->GetMaterialType())
            {
            case MaterialType::Default:
                ImGui::Text("Material type is Default");

                renderButtonMakeDefault = false;
                renderButtonMakeConfig = true;
                renderButtonMakeFile = true;
                break;

            case MaterialType::Config:
                EditorUtils::ImGuiColor("Color", material->GetColor());

                renderButtonMakeDefault = true;
                renderButtonMakeConfig = false;
                renderButtonMakeFile = true;

                break;

            case MaterialType::File:
                // todo:
                renderButtonMakeDefault = true;
                renderButtonMakeConfig = true;
                renderButtonMakeFile = false;
                break;

            case MaterialType::None:
                // note: what
                ImGui::TextColored({1.0, 0, 0, 1.0}, "ERROR");
                break;
            }
        }

        // todo: make make this a function
        if (renderButtonMakeDefault)
            if (ImGui::Button("Load Default Material"))
                mesh->SetMaterialToDefault();

        if (renderButtonMakeConfig)
            if (ImGui::Button("Load Material Config"))
                mesh->SetMaterial(Material::Configuration());

        // todo: file
    }

} // namespace Core
