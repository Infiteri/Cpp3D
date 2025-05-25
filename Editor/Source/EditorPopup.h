#pragma once

#include "Renderer/Material/Material.h"
#include "Renderer/Texture/CubemapTexture.h"
#include "Renderer/Texture/Texture2D.h"
#include <string>
namespace Core
{
    struct EditorPopup
    {
        bool Active = false;
        virtual void Render() {};
    };

    struct CeImageConvertorPopup : public EditorPopup
    {
        std::string Input = "";
        std::string Output = "";

        void Render();
    };

    struct CubemapConfigPopup : public EditorPopup
    {
        std::string ConfigPath;
        CubemapConfiguration Config;
        Texture2D Textures[6];

        void Render();
    };

    struct MaterialConfigPopup : public EditorPopup
    {
        std::string Path;
        Material::Configuration Config;

        void Render();
    };

    class EditorPopupSystem
    {
    public:
        CeImageConvertorPopup Image;
        CubemapConfigPopup Cubemap;
        MaterialConfigPopup Material;

        void OnImGuiRender();
    };
} // namespace Core
