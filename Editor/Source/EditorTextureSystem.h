#pragma once

#include "Base.h"
#include "Renderer/Texture/Texture2D.h"

#include <string>
#include <unordered_map>

namespace Core
{
    class EditorTextureSystem
    {
    public:
        struct State
        {
            std::unordered_map<std::string, Texture2D *> Tex;
        };

    public:
        static u32 GetTexID(const std::string &name);
    };
} // namespace Core
