#include "EditorPopup.h"
#include "Core/Util/StringUtils.h"
#include "EditorToast.H"
#include "EditorUtils.h"
#include "Platform/Platform.h"
#include "Resource/CeImageLoader.h"
#include "Resource/CubemapLoader.h"
#include "Resource/MaterialLoader.h"

#include <imgui.h>

namespace Core
{
    static void CeImageConvertorDragDropStr(std::string &str)
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
            {
                const char *path = (const char *)payload->Data;
                std::string ext = StringUtils::GetFilenameExtension(path);
                if (EditorUtils::StringIsImageExtension(ext))
                    str = path;
            }
            ImGui::EndDragDropTarget();
        }
    }

    void CeImageConvertorPopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Ce Image Convertor");

        ImGui::Text("This will convert the specified image to a 'ce_image'");

        EditorUtils::ImGuiString("Input Image", Input);
        CeImageConvertorDragDropStr(Input);

        EditorUtils::ImGuiString("Output Image", Output);
        CeImageConvertorDragDropStr(Output);

        if (ImGui::Button("Convert"))
        {
            CeImageLoader loader;

            if (StringUtils::GetFilenameExtension(Output) != "ce_image")
                Output += ".ce_image";

            loader.CreateCeImage(Input, Output);

            Active = false;
            Input = "";
            Output = "";
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            Active = false;
            Input = "";
            Output = "";
        }

        ImGui::End();
    }

    static void CubemapConfigPopupRenderImage(Texture2D *texture, const ImVec2 &size,
                                              std::string &path)
    {
        if (!texture)
            return;

        ImGui::Image((void *)(u64)(u32)texture->GetID(), size);

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
            {
                const char *str = (const char *)payload->Data;
                std::string ext = StringUtils::GetFilenameExtension(str);
                if (EditorUtils::StringIsImageExtension(ext))
                {
                    path = str;
                    texture->Destroy();
                    texture->Load(path);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    void CubemapConfigPopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Cubemap Configurator");

        const ImVec2 size = {150, 150};
        const float spacing = 4.0f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + size.x + spacing);
        CubemapConfigPopupRenderImage(&Textures[2], size, Config.Top);

        // Middle row: [2] [5] [1] [6]
        CubemapConfigPopupRenderImage(&Textures[1], size, Config.Left);

        ImGui::SameLine(0.0f, spacing);
        CubemapConfigPopupRenderImage(&Textures[4], size, Config.Front);

        ImGui::SameLine(0.0f, spacing);
        CubemapConfigPopupRenderImage(&Textures[0], size, Config.Right);

        ImGui::SameLine(0.0f, spacing);
        CubemapConfigPopupRenderImage(&Textures[5], size, Config.Back);

        // Bottom row:      [  -  4  -  ]
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + size.x + spacing);
        CubemapConfigPopupRenderImage(&Textures[3], size, Config.Bottom);

        ImGui::NewLine();

        ImGui::SeparatorText("Configuration");

        EditorUtils::ImGuiString("Right", Config.Right);
        EditorUtils::ImGuiString("Left", Config.Left);
        EditorUtils::ImGuiString("Top", Config.Top);
        EditorUtils::ImGuiString("Bottom", Config.Bottom);
        EditorUtils::ImGuiString("Front", Config.Front);
        EditorUtils::ImGuiString("Back", Config.Back);

        if (ImGui::Button("Save"))
        {
            ToastMessage::Add("Saved cubemap");
            CubemapLoader loader;
            loader.Serialize(Config, ConfigPath);
            Active = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Save Elsewhere"))
        {
            std::string path = Platform::SaveFileDialog("*.ce_cubemap \0*.ce_cubemap\0");
            if (!path.empty())
            {
                CubemapLoader loader;
                loader.Serialize(Config, path);
                Active = false;
            }

            ToastMessage::Add("Saved cubemap");
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            Active = false;
        }

        ImGui::End();
    }

    void MaterialConfigPopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Material Configurator");

        EditorUtils::RenderMaterialConfig(Config);

        if (ImGui::Button("Save"))
        {
            Active = false;
            MaterialLoader loader;
            loader.Serialize(Path, Config);
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            Active = false;

        ImGui::End();
    }

    void EditorPopupSystem::OnImGuiRender()
    {
        Image.Render();
        Cubemap.Render();
        Material.Render();
    }

} // namespace Core
