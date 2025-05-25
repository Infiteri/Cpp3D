#pragma once

#include "Base.h"
#include <vector>

namespace Core
{
    class FrameBuffer
    {
    public:
        enum TextureType
        {
            Depth,
            Rgba8,
            Rgb,
            R32I
        };

        struct RenderPass
        {
            TextureType Type;
            u32 Index;
            u32 Id;

            RenderPass(TextureType t) : Type(t) {};
        };

        struct Configuration
        {
            int Width;
            int Height;

            std::vector<RenderPass> Passes;
        };

    private:
        u32 id;
        Configuration state;

        void PrepareForResize();

    public:
        FrameBuffer(const Configuration &config);
        ~FrameBuffer();

        void Bind();
        void Unbind();
        void Create();
        void Destroy();

        int ReadPixel(u32 attachment, int x, int y);

        void Resize(int w, int h);

        RenderPass *GetRenderPass(int index);
        inline u32 GetID() const { return id; };
    };
} // namespace Core
