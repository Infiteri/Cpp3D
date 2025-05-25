#include "ContentBrowserPanel.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Logger.h"
#include "Core/Util/StringUtils.h"
#include "EditorUtils.h"
#include "Platform/Platform.h"

#include "Renderer/Texture/TextureSystem.h"
#include "Resource/CubemapLoader.h"
#include "Resource/MaterialLoader.h"

#include <algorithm>
#include <cstdlib>
#include <imgui.h>

namespace Core
{
    // todo: As intended
    static bool firstLoad = true;

    void ContentBrowserPanel::CreateFilePopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Create File Popup");

        ImGui::Text("Will Create a new file under the specified configuration ");
        ImGui::Text("The final format is 'FolderPath/FilePath'");

        std::string tf = TargetFolder;
        if (EditorUtils::ImGuiString("Folder Path", tf))
            TargetFolder = tf;

        EditorUtils::ImGuiString("File Path", File);

        if (ImGui::Button("Create"))
        {
            FileSystem::CreateFile(TargetFolder + "/" + File);
            Active = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            Active = false;

        ImGui::End();
    }

    void ContentBrowserPanel::CreateMaterialPopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Create Material Popup");

        ImGui::Text("Will Create a new material under the specified configuration and file path");
        ImGui::Text("The final format is 'FolderPath/FilePath'");

        std::string tf = TargetFolder;
        if (EditorUtils::ImGuiString("Folder Path", tf))
            TargetFolder = tf;

        EditorUtils::ImGuiString("File Path", File);

        ImGui::SeparatorText("Material Configuration");

        EditorUtils::RenderMaterialConfig(Config);

        if (ImGui::Button("Create"))
        {
            if (StringUtils::GetFilenameExtension(File) != "ce_mat")
                File += ".ce_mat"; // note: Ensures the file ends in ce_mat

            Material m(Config); // TODO: PLEASE FIX THIS BS
            MaterialLoader loader;
            loader.Serialize(TargetFolder + "/" + File, &m);
            Active = false;

            if (Texture != TextureSystem::GetDefault())
                delete Texture;
            Texture = nullptr;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            Active = false;

        ImGui::End();
    }

    static bool CubemapPopupStringInput(std::string &input, const char *label)
    {
        EditorUtils::ImGuiString(label, input);

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
            {
                const char *path = (const char *)payload->Data;
                std::string ext = StringUtils::GetFilenameExtension(path);

                if (EditorUtils::StringIsImageExtension(ext))
                {
                    input = path;
                    ImGui::EndDragDropTarget();
                    return true;
                }
            }

            ImGui::EndDragDropTarget();
        }

        return false;
    }

    void ContentBrowserPanel::CreateCubemapPopup::Render()
    {
        if (!Active)
            return;

        ImGui::Begin("Create Cubemap File");

        ImGui::Text("Will Create a new cubemap under the specified configuration ");
        ImGui::Text("The final format is 'FolderPath/FilePath'");

        EditorUtils::ImGuiString("Folder Path", TargetFolder);
        EditorUtils::ImGuiString("File Path", File);

        CubemapPopupStringInput(Config.Right, "Right");
        CubemapPopupStringInput(Config.Left, "Left");
        CubemapPopupStringInput(Config.Top, "Top");
        CubemapPopupStringInput(Config.Bottom, "Bottom");
        CubemapPopupStringInput(Config.Front, "Front");
        CubemapPopupStringInput(Config.Back, "Back");

        if (ImGui::Button("Create"))
        {
            Active = false;
            CubemapLoader l;
            l.Serialize(Config, TargetFolder + "/" + File); // todo: ext check

            Config = {};
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            Active = false;
            Config = {};
        }

        ImGui::End();
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        if (firstLoad)
        {
            firstLoad = false;

            state.FolderTexture.Load("EngineAssets/Icons/folder.png");
            state.IconTexture.Load("EngineAssets/Icons/icon.png");
        }

        ImGui::Begin("Content Browser");

        float panelWidth = ImGui::GetContentRegionAvail().x;
        float cellSize = state.ThumbnailSize + state.Padding;
        int columnCount = (int)(panelWidth / cellSize);
        columnCount = columnCount < 1 ? 1 : columnCount;
        ImVec2 size{state.ThumbnailSize, state.ThumbnailSize}; // Might change based on preferences
        ImGui::Columns(columnCount, 0, false);

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) &&
            !ImGui::IsAnyItemHovered())
            ImGui::OpenPopup("ContentPanelRightClick");

        if (ImGui::BeginPopup("ContentPanelRightClick"))
        {

            if (ImGui::MenuItem("Paste"))
            {
                if (!state.CopyFilePath.empty())
                {
                    std::string filename = StringUtils::GetFilenameFromPath(state.CopyFilePath);
                    std::string destination = state.CurrentDirectroy + "/" + filename;
                    FileSystem::CopyFile(state.CopyFilePath, destination);
                }
            }

            if (ImGui::MenuItem("Create File"))
            {
                state.CreateFile.File = "";
                state.CreateFile.TargetFolder = state.CurrentDirectroy;
                state.CreateFile.Active = true;
            }

            if (ImGui::MenuItem("Create Material"))
            {
                state.CreateMaterial.File = "";
                state.CreateMaterial.TargetFolder = state.CurrentDirectroy;
                state.CreateMaterial.Config = {};
                state.CreateMaterial.Texture = nullptr;
                state.CreateMaterial.NormalTexture = nullptr;
                state.CreateMaterial.Active = true;
            }

            if (ImGui::MenuItem("Create Cubemap"))
            {
                state.CreateCubemap.File = "";
                state.CreateCubemap.TargetFolder = state.CurrentDirectroy;
                state.CreateCubemap.Config = {};
                state.CreateCubemap.Active = true;
            }

            if (ImGui::MenuItem("Open in File Explorer"))
            {
                std::string cd = state.CurrentDirectroy;
                // note: replace forward slashes with backshot backward slashes
                // note: dear future employer if you see this, 'backshot slashes' is fun for some
                // reason

                std::replace(cd.begin(), cd.end(), '/', '\\');
                system((std::string("explorer ") + cd).c_str());
            }
            ImGui::End();
        }

        if (state.CurrentDirectroy != state.BaseDirectory && ImGui::Button("<-"))
            state.CurrentDirectroy = state.BaseDirectory;

        if (!state.CurrentDirectroy.empty())
        {
            int it = 0;
            for (auto path : Platform::GetDirectoryEntries(state.CurrentDirectroy))
            {
                ImGui::PushID(++it);
                Texture2D *texture = path.IsFolder ? &state.FolderTexture : &state.IconTexture;

                if (ImGui::ImageButton((void *)(u64)texture->GetID(), size))
                    if (path.IsFolder)
                        state.CurrentDirectroy += "/" + path.Name;

                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                {
                    state.RightClickEntry.Name = path.Name;
                    state.RightClickEntry.IsFolder = path.IsFolder;
                    ImGui::OpenPopup("ContentPanelRightItem");
                }

                if (ImGui::BeginPopup("ContentPanelRightItem"))
                {
                    if (ImGui::MenuItem("Copy"))
                    {
                        state.CopyFilePath = path.Name;
                    }

                    if (ImGui::MenuItem("Delete"))
                    {
                        FileSystem::RemoveFile(state.RightClickEntry.Name);
                        state.RightClickEntry.Name = "";
                        state.RightClickEntry.IsFolder = false;
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("ContentPanelDragDrop", path.Name.c_str(),
                                              path.Name.size() + 1);
                    ImGui::EndDragDropSource();
                }

                if (!path.IsFolder)
                {
                    std::string filePath;
                    size_t lastSlash = path.Name.find_last_of("/\\");
                    if (lastSlash != std::string::npos)
                        filePath = path.Name.substr(lastSlash + 1);
                    ImGui::TextWrapped(filePath.c_str());
                }
                else
                    ImGui::TextWrapped(path.Name.c_str());

                ImGui::NextColumn();
                ImGui::PopID();
            }
        }

        ImGui::Columns(1);
        ImGui::End();

        state.CreateFile.Render();
        state.CreateMaterial.Render();
        state.CreateCubemap.Render();
    }
} // namespace Core
