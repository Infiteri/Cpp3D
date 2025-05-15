#include "TextureSystem.h"
#include "Core/Logger.h"

#define CE_DEBUG_TEXTURE_SYS 1

#if CE_DEBUG_TEXTURE_SYS == 1
#define CE_TEX_DBG(msg, ...) CE_LOG("CE_RENDER", Trace, msg, ##__VA_ARGS__)
#else
#define CE_TEX_DBG(msg, ...)
#endif

namespace Core
{
    static TextureSystem::State state;

    void TextureSystem::Init()
    {
        u8 data[4] = {255, 255, 255, 255};

        state.DefaultTexture.Load(1, 1, 4, data);
    }

    void TextureSystem::Shutdown()
    {
        for (auto &ref : state.Tex2D)
        {
            if (ref.second.Count > 0)
                CE_LOG("CE_RENDER", Warn,
                       "Texture reference count greater than zero, should have been removed.");

            delete ref.second.Texture;
        }
    }

    bool TextureSystem::Exists(const std::string &name)
    {
        return state.Tex2D.find(name) != state.Tex2D.end();
    }

    bool TextureSystem::IsTextureDefault(Texture2D *texture)
    {
        return texture == &state.DefaultTexture;
    }

    Texture2D *TextureSystem::GetDefault() { return &state.DefaultTexture; }

    Texture2D *TextureSystem::Get(const std::string &name)
    {
        if (!Exists(name))
            return Load(name);

        state.Tex2D[name].Count++;
        CE_TEX_DBG("Getting texture: '%s', new count = %i", name.c_str(), state.Tex2D[name].Count);
        return state.Tex2D[name].Texture;
    }

    Texture2D *TextureSystem::Load(const std::string &name)
    {
        if (Exists(name))
        {
            CE_LOG("CE_RENDER", Warn, "Texture '%s' exists, nothing done in TextureSystem::Load.",
                   name.c_str());
            return Get(name);
        }

        state.Tex2D[name] = {1};
        state.Tex2D[name].Texture = new Texture2D;
        state.Tex2D[name].Texture->Load(name);
        CE_TEX_DBG("Loaded texture: '%s'", name.c_str());

        return state.Tex2D[name].Texture;
    }

    void TextureSystem::Remove(const std::string &name)
    {
        if (!Exists(name))
            return;

        state.Tex2D[name].Count--;
        CE_TEX_DBG("Removing texture: '%s', new count = %i", name.c_str(), state.Tex2D[name].Count);
        if (state.Tex2D[name].Count <= 0)
        {
            delete state.Tex2D[name].Texture;
            state.Tex2D.erase(name);
        }
    }
} // namespace Core
