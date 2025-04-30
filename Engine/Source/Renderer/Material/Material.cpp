#include "Material.h"
#include "Base.h"
#include "Core/Logger.h"
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

    void Material::ClearTexture(Texture2D *texture)
    {
        if (colorTextureType == TextureType::None)
        {
            CE_LOG("CE_RENDER", Error, "Texture type cannot be None.");
            return;
        }

        if (colorTextureType == TextureType::Default)
            return;

        TextureSystem::Remove(colorTexture->GetImagePath());
        colorTexture = nullptr;
        colorTextureType = TextureType::None;
    }

    void Material::SetColorTextureDefault()
    {
        colorTextureType = TextureType::Default;
        colorTexture = TextureSystem::GetDefault();
    }

    void Material::SetColorTexture(const std::string &name)
    {
        colorTextureType = TextureType::Loaded;
        colorTexture = TextureSystem::Get(name);
    }

    void Material::Use(Shader *shader)
    {
        CE_VERIFY(shader);

        shader->Vec4(state.Color, "uColor");

        CE_VERIFY(colorTexture);
        colorTexture->Use();
        shader->Int(0, "uColorTexture");
    }

} // namespace Core
