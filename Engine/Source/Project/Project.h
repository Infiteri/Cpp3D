#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    struct ProjectConfiguration
    {
        std::string Name = "Untitled";

        /// @brief asset Path, all assets are relative to this path
        std::string AssetPath = ".";

        /// @brief The starting scene name
        std::string StartScene = "Scene.ce_scene";

        /// @brief The game library path. Used for the script engine and all other game code
        /// related.
        std::string LibraryPath = "Lib.dll";
    };

    class CE_API Project
    {
    private:
        ProjectConfiguration state;

    public:
        Project() = default;
        ~Project() = default;

        inline ProjectConfiguration &GetState() { return state; };

        inline std::string GetName() { return state.Name; };
        inline std::string GetAssetPath() { return state.AssetPath; };
        inline std::string GetStartScene() { return state.AssetPath + "/" + state.StartScene; };
        inline std::string GetLibraryPath() { return state.AssetPath + "/" + state.LibraryPath; };
    };
} // namespace Core
