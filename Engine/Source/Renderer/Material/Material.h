#pragma once

#include "Base.h"
#include "Renderer/Color.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture/Texture2D.h"

namespace Core
{
    /// @brief Represents the type of material, just the state of the material.
    /// @note Needed for editing materialss.
    enum class MaterialType
    {
        None = 0,
        Default,
        File,
        Config
    };

    class CE_API Material
    {
    public:
        struct Configuration
        {
            /// Name of the material, also used as the identifier for the MaterialSystem.
            std::string Name;

            /// Default color of the material.
            Color Color;

            Configuration() = default;
            Configuration(const Configuration &other);
        };

    private:
        enum class TextureType
        {
            None = 0,
            Default,
            Loaded
        };

        struct TexPair
        {
            Texture2D *Tex;
            TextureType Type;

            TexPair() : Tex(nullptr), Type(TextureType::None) {};
        };

    private:
        Configuration state;

        TexPair colorTexture;

        void ClearTexture(TexPair &pair);

    public:
        Material();
        Material(const Configuration &config);
        ~Material();

        void SetColorTextureDefault();
        void SetColorTexture(const std::string &name);

        void Use(Shader *shader);

        inline Color &GetColor() { return state.Color; };
        inline const std::string &GetName() { return state.Name; };
    };
} // namespace Core
