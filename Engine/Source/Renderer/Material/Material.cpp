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
        Metallic = other.Metallic;
        Roughness = other.Roughness;
        AO = other.AO;
        ColorTexture = other.ColorTexture;
        NormalTexture = other.NormalTexture;
    }

    Material::Material()
    {
        SetColorTextureDefault();
        SetNormalTextureDefault();
    }

    Material::Material(const Configuration &config)
    {
        state = config;

        Metallic = state.Metallic;
        Roughness = state.Roughness;
        AO = state.AO;

        colorTexture.Tex = nullptr;
        SetColorTextureDefault();

        normalTexture.Tex = nullptr;
        SetNormalTextureDefault();

        if (!config.ColorTexture.empty())
            SetColorTexture(config.ColorTexture);

        if (!config.NormalTexture.empty())
            SetNormalTexture(config.NormalTexture);
    }

    Material::~Material()
    {
        ClearTexture(colorTexture);
        ClearTexture(normalTexture);
    }

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
        p.Tex = nullptr;
        p.Type = TextureType::None;
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
        if (this == MaterialSystem::GetDefault())
        {
            CE_LOG("CE_RENDER", Warn,
                   "Cannot set color texture, material must be set to Config type");
            return;
        }

        ClearTexture(colorTexture);

        colorTexture.Type = TextureType::Loaded;
        colorTexture.Tex = TextureSystem::Get(name);
    }

    void Material::SetNormalTextureDefault()
    {
        if (normalTexture.Tex)
            ClearTexture(normalTexture);

        normalTexture.Type = TextureType::Default;
        normalTexture.Tex = TextureSystem::GetDefault();
    }

    void Material::SetNormalTexture(const std::string &name)
    {
        if (this == MaterialSystem::GetDefault())
        {
            CE_LOG("CE_RENDER", Warn,
                   "Cannot set color texture, material must be set to Config type");
            return;
        }

        ClearTexture(normalTexture);

        normalTexture.Type = TextureType::Loaded;
        normalTexture.Tex = TextureSystem::Get(name);
    }

    void Material::Use(Shader *shader)
    {
        CE_VERIFY(shader);
        CE_VERIFY(colorTexture.Tex);
        CE_VERIFY(normalTexture.Tex);

        shader->Vec4(state.Color, "uColor");

        shader->Float(AO, "uAO");
        shader->Float(Roughness, "uRoughness");
        shader->Float(Metallic, "uMetallic");

        colorTexture.Tex->Use();
        shader->Int(0, "uColorTexture");

        normalTexture.Tex->Use(1);
        shader->Int(1, "uNormalTexture");
    }
} // namespace Core
