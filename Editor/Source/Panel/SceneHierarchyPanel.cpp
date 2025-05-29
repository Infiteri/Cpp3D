#include "SceneHierarchyPanel.h"
#include "Base.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Util/StringUtils.h"
#include "EditorLayer.h"
#include "EditorUtils.h"
#include "Platform/Platform.h"
#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Texture/TextureSystem.h"
#include "Scene/Components/Components.h"
#include "Scene/World.h"

#include <imgui.h>
#include <string>

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

#define CE_ADD_COMP_RENDER(label, type)                                                            \
    if (ImGui::MenuItem(label))                                                                    \
    a->AddComponent<type>()
namespace Core
{
    static int actorIndex = 0; // hack: Really hacked in

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy Panel");

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Create New Actor"))
            {
                if (!World::GetActive())
                    CE_LOG_NO_ACTIVE_SCENE();
                else
                    World::GetActive()->CreateActor("Actor");
            }

            ImGui::EndPopup();
        }

        actorIndex = 0;
        if (World::GetActive())
            for (auto actor : World::GetActive()->GetActors())
            {
                RenderActor(actor);
                actorIndex++;
            }

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
        std::string uniquePopup =
            std::string("ActorRightClickPopup" + std::to_string(a->GetID().Get()));

        if (!parent || parentNodeOpen)
        {
            pop = ImGui::TreeNodeEx((void *)(u64)a->GetID(), flags, a->GetName().c_str());
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                selected = a;

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                ImGui::OpenPopup(uniquePopup.c_str());
        }

        bool deleteActor = false;
        if (ImGui::BeginPopupContextItem(uniquePopup.c_str()))
        {
            if (ImGui::MenuItem("Delete Actor"))
                deleteActor = true;

            ImGui::EndPopup();
        }

        // note: Should implement drag drop for parenting
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("ActorDragDropHierarchy", &a->GetID(), sizeof(UUID));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload =
                    ImGui::AcceptDragDropPayload("ActorDragDropHierarchy"))
            {
                UUID *id = (UUID *)payload->Data;

                auto moveActor = World::GetActive()->GetActorInAllHierarchy(UUID(id->Get()));
                if (moveActor)
                {
                    if (!moveActor->GetActor(a->GetID()))
                    {
                        if (moveActor->GetParent() != nullptr)
                            moveActor->GetParent()->RemoveActor(UUID(id->Get()));
                        else
                            World::GetActive()->RemoveActor(UUID(id->Get()));
                    }

                    a->AddChild(moveActor);
                }
                else
                    CE_ERROR("Couldn't find actor to move in hierarchy");
            }

            ImGui::EndDragDropTarget();
        }

        if (!parent || parentNodeOpen)
            ImGui::Dummy({ImGui::GetWindowWidth(), 5});

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("ActorDragDropHierarchy", &a->GetID(), sizeof(UUID));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload =
                    ImGui::AcceptDragDropPayload("ActorDragDropHierarchy"))
            {
                UUID *uid = (UUID *)payload->Data;
                UUID rawUUID = UUID(uid->Get());

                auto moveActor = World::GetActive()->GetActorInAllHierarchy(rawUUID);
                if (moveActor)
                {
                    if (!moveActor->GetParent())
                        World::GetActive()->MoveActorInHierarchy(moveActor->GetID(), actorIndex);
                    else
                    {
                        moveActor->GetParent()->RemoveActor(moveActor->GetID());
                        World::GetActive()->AddActor(moveActor);
                        World::GetActive()->MoveActorInHierarchy(moveActor->GetID(), actorIndex);
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }

        for (auto child : a->GetChildren())
            RenderActor(child, a, pop);

        if (pop)
            ImGui::TreePop();

        if (deleteActor)
        {
            if (selected)
                if (selected->GetID() == a->GetID())
                    selected = nullptr;

            // todo: ...
            if (a->GetParent())
                a->GetParent()->RemoveActor(a->GetID(), true);
            else
                World::GetActive()->RemoveActor(a->GetID(), true);
        }
    }

    void RenderMeshUI(MeshComponent *m, Actor *a);
    void RenderPointLightUI(PointLightComponent *c, Actor *a);
    void RenderSpotLightUI(SpotLightComponent *c, Actor *a);
    void RenderPerspectiveCameraUI(PerspectiveCameraComponent *c, Actor *a);
    void ScriptComponentUI(ScriptComponent *c, Actor *a);

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
        CE_RENDER_COMP("Point Light Component", PointLightComponent, RenderPointLightUI);
        CE_RENDER_COMP("Spot Light Component", SpotLightComponent, RenderSpotLightUI);
        CE_RENDER_COMP("Perspective Camera Component", PerspectiveCameraComponent,
                       RenderPerspectiveCameraUI);
        CE_RENDER_COMP("Script Component", ScriptComponent, ScriptComponentUI);

        ImGui::NewLine();

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponentPopup");

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            CE_ADD_COMP_RENDER("Mesh Component", MeshComponent);
            CE_ADD_COMP_RENDER("Point Light Component", PointLightComponent);
            CE_ADD_COMP_RENDER("Spot Light Component", SpotLightComponent);
            CE_ADD_COMP_RENDER("Perspective Camera Component", PerspectiveCameraComponent);
            CE_ADD_COMP_RENDER("Script Component", ScriptComponent);

            ImGui::EndPopup();
        }
    }

    enum class TextureUIType
    {
        Color,
        Normal
    };

    void RenderMeshTextureUI(MeshComponent *m, Actor *a, TextureUIType t = TextureUIType::Color)
    {
        std::string treeTitle = "Color Texture";
        if (t == TextureUIType::Normal)
            treeTitle = "Normal Texture";

        if (ImGui::TreeNode(treeTitle.c_str()))
        {
            Texture2D *tex = nullptr;

            switch (t)
            {
            case TextureUIType::Color:
                tex = m->GetMesh()->GetMaterial()->GetColorTexture();
                break;

            case TextureUIType::Normal:
                tex = m->GetMesh()->GetMaterial()->GetNormalTexture();
                break;
            }

            if (tex == nullptr)
            {
                ImGui::TreePop();
                return;
            }

            const ImVec2 size = {100, 100}; // todo: Size type shit

            ImGui::NewLine();
            ImGui::Image((void *)(u64)tex->GetID(), size);

            // note: Here you can accept ImGui drag drop image data
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload =
                        ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
                {
                    const char *path = (const char *)payload->Data;
                    std::string ext = StringUtils::GetFilenameExtension(path);

                    // todo: Some hashmap could work
                    if (EditorUtils::StringIsImageExtension(ext))
                        switch (t)
                        {
                        case TextureUIType::Color:
                            m->GetMesh()->GetMaterial()->SetColorTexture(path);
                            break;

                        case TextureUIType::Normal:
                            m->GetMesh()->GetMaterial()->SetNormalTexture(path);
                            break;
                        }
                }

                ImGui::EndDragDropTarget();
            }
            ImGui::NewLine();

            // note: Render texture information
            if (TextureSystem::IsTextureDefault(tex))
            {
                ImGui::Text("Texture is the default color texture.");
            }
            else
            {
                std::string path = tex->GetImagePath();
                if (!path.empty())
                {
                    ImGui::Text("%s", path.c_str());
                }
            }

            if (ImGui::Button("Load"))
            {
                std::string src =
                    Platform::OpenFileDialog("Image (*.png *.jpg *.jpeg)\0*.png;*.jpg;*.jpeg\0");

                if (!src.empty())
                    switch (t)
                    {
                    case TextureUIType::Color:
                        m->GetMesh()->GetMaterial()->SetColorTexture(src);
                        break;

                    case TextureUIType::Normal:
                        m->GetMesh()->GetMaterial()->SetNormalTexture(src);
                        break;
                    }
            }

            ImGui::SameLine();
            if (ImGui::Button("Default"))
                switch (t)
                {
                case TextureUIType::Color:
                    m->GetMesh()->GetMaterial()->SetColorTextureDefault();
                    break;

                case TextureUIType::Normal:
                    m->GetMesh()->GetMaterial()->SetNormalTextureDefault();
                    break;
                }

            ImGui::TreePop();
        }
    }

    void RenderMeshUI(MeshComponent *m, Actor *a)
    {
        CE_VERIFY(m && a);
        // note: material

        if (ImGui::TreeNode("Material"))
        {

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

                    ImGui::DragFloat("Metallic", &material->Metallic, 0.01, 0.01);
                    ImGui::DragFloat("Roughness", &material->Roughness, 0.01, 0.01);
                    ImGui::DragFloat("AO", &material->AO, 0.01, 0.01);

                    renderButtonMakeDefault = true;
                    renderButtonMakeConfig = false;
                    renderButtonMakeFile = true;

                    RenderMeshTextureUI(m, a);
                    RenderMeshTextureUI(m, a, TextureUIType::Normal);

                    break;

                case MaterialType::File:
                    // todo:
                    renderButtonMakeDefault = true;
                    renderButtonMakeConfig = true;
                    renderButtonMakeFile = true;

                    ImGui::Text("Material is file loaded");
                    ImGui::Text("%s", mesh->GetMaterial()->GetName().c_str());
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

            if (renderButtonMakeFile)
            {
                if (ImGui::Button("Load From File"))
                {
                    std::string path = Platform::OpenFileDialog("Material \0*.ce_mat\0");
                    if (!path.empty())
                        mesh->SetMaterial(path);
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload =
                            ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
                    {
                        const char *path = (const char *)payload->Data;
                        std::string ext = StringUtils::GetFilenameExtension(path);

                        if (ext == "ce_mat")
                            mesh->SetMaterial(path);
                    }

                    ImGui::EndDragDropTarget();
                }
            }

            // todo: file
            //
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Geometry"))
        {
            auto geometry = m->GetMesh()->GetGeometry();

            switch (geometry->GetType())
            {
            case Geometry::Box:
            {
                auto box = (BoxGeometry *)geometry;
                float data[3] = {box->Width, box->Height, box->Depth};

                if (ImGui::DragFloat3("Size", data, 0.02f, 0.0f))
                    m->GetMesh()->SetGeometry(new BoxGeometry(data[0], data[1], data[2]));
            }
            break;

            default:
                break;
            }

            // todo: Geometry type picker when multiple geometries exist?

            ImGui::TreePop();
        }
    }

    void RenderPointLightUI(PointLightComponent *c, Actor *a)
    {
        auto pl = &c->Light;

        EditorUtils::ImGuiColor("Color", pl->Color);
        EditorUtils::ImGuiVector3("Specular", pl->Specular);
        ImGui::DragFloat("Constant", &pl->Constant, 0.02, 0.0f);
        ImGui::DragFloat("Linear", &pl->Linear, 0.02, 0.0f);
        ImGui::DragFloat("Quadratic", &pl->Quadratic, 0.02, 0.0f);
        ImGui::DragFloat("Radius", &pl->Radius, 0.02, 0.0f);
        ImGui::DragFloat("Intensity", &pl->Intensity, 0.02, 0.0f);
    }

    void RenderSpotLightUI(SpotLightComponent *c, Actor *a)
    {

        auto pl = &c->Light;

        EditorUtils::ImGuiColor("Color", pl->Color);
        EditorUtils::ImGuiVector3("Specular", pl->Specular);
        EditorUtils::ImGuiVector3("Diffuse", pl->Diffuse);
        EditorUtils::ImGuiVector3("Direction", pl->Direction);
        EditorUtils::ImGuiVector3("Position", pl->Position);
        ImGui::DragFloat("Constant", &pl->Constant, 0.02, 0.0f);
        ImGui::DragFloat("Linear", &pl->Linear, 0.02, 0.0f);
        ImGui::DragFloat("Quadratic", &pl->Quadratic, 0.02, 0.0f);
        ImGui::DragFloat("Cut off", &pl->CutOff, 0.02, 0.0f);
        ImGui::DragFloat("Outer cut off", &pl->OuterCutOff, 0.02, 0.0f);
    }

    void RenderPerspectiveCameraUI(PerspectiveCameraComponent *c, Actor *a)
    {
        bool funcs[] = {
            ImGui::DragFloat("FOV", &c->FOV, 0.01f, 15.0f, 360.0f),
            ImGui::DragFloat("Near", &c->Near, 0.01f, 0.0001f),
            ImGui::DragFloat("Far", &c->Far, 0.01f, 0.001f),
        };

        ImGui::Checkbox("Is Primary", &c->IsPrimary);

        // note : Too much?
        for (int i = 0; i < 3; i++)
            if (funcs[i])
                c->UpdateCamera();
    }

    void ScriptComponentUI(ScriptComponent *c, Actor *a)
    {
        EditorUtils::ImGuiString("Class Name", c->ClassName);
    }
} // namespace Core
