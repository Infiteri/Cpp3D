#pragma once

#include "Panel.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Texture/Texture2D.h"
#include <string>

namespace Core
{
    class ContentBrowserPanel : public Panel
    {
    public:
        struct CreateFilePopup
        {
            bool Active = false;
            std::string File;
            std::string TargetFolder;
            void Render();
        };

        struct CreateMaterialPopup
        {
            Material::Configuration Config;
            Texture2D *Texture = nullptr;
            bool Active = false;
            std::string File;
            std::string TargetFolder;
            void Render();
        };

        struct RightClikedEntry
        {
            std::string Name;
            bool IsFolder;
        };

        struct State
        {
            float ThumbnailSize = 128.0f;
            float Padding = 16.0f;

            std::string BaseDirectory = "Assets";
            std::string CurrentDirectroy = "Assets";
            std::string CopyFilePath = "";

            Texture2D IconTexture, FolderTexture;

            CreateFilePopup CreateFile;
            CreateMaterialPopup CreateMaterial;
            RightClikedEntry RightClickEntry;
        };

        State state;

    public:
        ContentBrowserPanel() {};
        ~ContentBrowserPanel() {};

        void OnImGuiRender();
    };
} // namespace Core
