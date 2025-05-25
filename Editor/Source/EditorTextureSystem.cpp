#include "EditorTextureSystem.h"
#include "Base.h"

#include "Renderer/Texture/TextureSystem.h"

namespace Core
{
    static EditorTextureSystem::State state;

    u32 EditorTextureSystem::GetTexID(const std::string &name)
    {
        if (name.empty())
            return TextureSystem::GetDefault()->GetID();

        if (state.Tex.find(name) == state.Tex.end())
        {
            state.Tex[name] = new Texture2D();
            state.Tex[name]->Load(name);
        }

        Texture2D *tex = state.Tex[name];
        if (!tex)
            return 0; // should never happen tho

        return tex->GetID();
    }
} // namespace Core
