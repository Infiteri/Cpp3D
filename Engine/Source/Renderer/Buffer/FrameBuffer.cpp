#include "FrameBuffer.h"
#include "Base.h"
#include "Core/Logger.h"

#include <glad/glad.h>

namespace Core
{
    static inline GLenum ClearToGL(FrameBuffer::TextureType type)
    {
        switch (type)
        {
        case FrameBuffer::Rgba8:
            return GL_RGBA8;
            break;

        case FrameBuffer::Rgb:
            return GL_RGB;
            break;

        case FrameBuffer::Depth:
            return GL_DEPTH24_STENCIL8;
            break;

        case FrameBuffer::R32I:
            return GL_RED_INTEGER;
            break;
        }

        return GL_RGB;
    }

    static inline GLenum FormatToGL(FrameBuffer::TextureType type)
    {
        switch (type)
        {
        case FrameBuffer::Rgba8:
            return GL_RGBA;
            break;

        case FrameBuffer::Rgb:
            return GL_RGB;
            break;

        case FrameBuffer::Depth:
            return GL_DEPTH24_STENCIL8;
            break;

        case FrameBuffer::R32I:
            return GL_R32I;
            break;
        }

        return GL_RGB;
    }

    FrameBuffer::FrameBuffer(const FrameBuffer::Configuration &config)
    {
        state = config;
        Create();
    }

    FrameBuffer::~FrameBuffer() { Destroy(); }

    void FrameBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }

    void FrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void FrameBuffer::Create()
    {
        glGenFramebuffers(1, &id);
        Bind();

        // generate passes
        for (u64 i = 0; i < state.Passes.size(); i++)
        {
            RenderPass *pass = &state.Passes[i];
            pass->Index = i;

            if (pass->Type == Depth)
            {
                glGenRenderbuffers(1, &pass->Id);
                glBindRenderbuffer(GL_RENDERBUFFER, pass->Id);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, state.Width,
                                      state.Height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER, pass->Id);
            }
            else
            {
                glGenTextures(1, &pass->Id);
                glBindTexture(GL_TEXTURE_2D, pass->Id);

                glTexImage2D(GL_TEXTURE_2D, 0, FormatToGL(pass->Type), state.Width, state.Height, 0,
                             ClearToGL(pass->Type), GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glBindTexture(GL_TEXTURE_2D, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                       pass->Id, 0);
            }
        }
        Unbind();
    }

    void FrameBuffer::PrepareForResize() { Destroy(); }

    void FrameBuffer::Resize(int w, int h)
    {
        if (w == state.Width && h == state.Height)
            return;

        PrepareForResize();

        state.Width = w;
        state.Height = h;

        Create();
    }

    FrameBuffer::RenderPass *FrameBuffer::GetRenderPass(int index)
    {
        if (index >= 0 && index < state.Passes.size())
            return &state.Passes[index];
        return nullptr;
    }

    void FrameBuffer::Destroy()
    {
        glDeleteFramebuffers(1, &id);
        id = 0;

        for (auto &pass : state.Passes)
        {
            if (pass.Type == FrameBuffer::Depth)
                glDeleteRenderbuffers(1, &pass.Id);
            else
                glDeleteTextures(1, &pass.Id);

            pass.Id = 0;
            pass.Index = 0;
        }
    }

    int FrameBuffer::ReadPixel(u32 attachment, int x, int y)
    {
        // todo: Finish
        if (!(attachment < state.Passes.size()))
            return -1;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        int value = -1;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
        return value;
    }

} // namespace Core
