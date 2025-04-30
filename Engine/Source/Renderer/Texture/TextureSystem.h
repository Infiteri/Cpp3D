#pragma once

#include "Texture2D.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class TextureSystem
    {
    public:
        struct Texture2DReference
        {
            Texture2D Texture;
            int Count;

            Texture2DReference() : Count(0) {};
            Texture2DReference(const int count) : Count(count) {};
        };

        struct State
        {
            Texture2D DefaultTexture;

            std::unordered_map<std::string, Texture2DReference> Tex2D;
        };

    public:
        TextureSystem() {};
        ~TextureSystem() {};

        static void Init();
        static void Shutdown();

        static bool Exists(const std::string &name);

        static Texture2D *GetDefault();
        static Texture2D *Get(const std::string &name);

        static Texture2D *Load(const std::string &name);

        static void Remove(const std::string &name);
    };
} // namespace Core
