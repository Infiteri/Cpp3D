#include "Material.h"
#include "Base.h"
#include "Core/Logger.h"
#include "Renderer/Material/MaterialSystem.h"
#include "Renderer/Texture/TextureSystem.h"

namespace Core
{
    Material::Configuration::Configuration(const Material::Configuration &other)
    {
        Name = other.Name;
        Color = other.Color;
    }

    Material::Material() { SetColorTextureDefault(); }

    Material::Material(const Configuration &config)
    {
        state = config;
        SetColorTextureDefault();
    }

    Material::~Material() { ClearTexture(colorTexture); }

    void Material::ClearTexture(TexPair &p)
    {
        if (p.Type == TextureType::None)
        {
            CE_LOG("CE_RENDER", Error, "Texture type cannot be None.");
            return;
        }

        if (p.Type == TextureType::Default)
            return;

        TextureSystem::Remove(p.Tex->GetImagePath());
        colorTexture.Tex = nullptr;
        colorTexture.Type = TextureType::None;
    }

    void Material::SetColorTextureDefault()
    {
        if (colorTexture.Tex)
            ClearTexture(colorTexture);

        colorTexture.Type = TextureType::Default;
        colorTexture.Tex = TextureSystem::GetDefault();
    }

    void Material::SetColorTexture(const std::string &name)
    {
        ClearTexture(colorTexture);

        if (this == MaterialSystem::GetDefault())
        {
            CE_LOG("CE_RENDER", Warn,
                   "Cannot set color texture, material must be set to Config type");
            return;
        }

        colorTexture.Type = TextureType::Loaded;
        colorTexture.Tex = TextureSystem::Get(name);
    }

    void Material::Use(Shader *shader)
    {
        CE_VERIFY(shader);

        shader->Vec4(state.Color, "uColor");

        // Due to per instance draw (as bad as it is) generations are really needed
        CE_VERIFY(colorTexture.Tex);
        colorTexture.Tex->Use();
        shader->Int(0, "uColorTexture");
    }
} // namespace Core
