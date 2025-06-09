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
    };

    class CE_API Project
    {
    private:
        ProjectConfiguration state;

    public:
        Project() = default;
        ~Project() = default;

        inline ProjectConfiguration &GetState() { return state; };

        inline std::string GetStartScene() { return state.AssetPath + "/" + state.StartScene; };
    };
} // namespace Core
