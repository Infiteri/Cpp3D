#include "Texture2D.h"
#include "Core/Logger.h"

#include <glad/glad.h>

namespace Core
{

    void Texture2D::EnsureDestructionID()
    {
        if (id != 0)
            glDeleteTextures(1, &id);

        id = 0;
    }

    // todo: actually make sure it works yk as the image deletion is dangerous

    Texture2D::Texture2D() { imageLoadPath = ""; }

    Texture2D::~Texture2D() { Destroy(); }

    void Texture2D::Load()
    {
        imageLoadPath = "";
        EnsureDestructionID();

        glGenTextures(1, &id);
    }

    void Texture2D::Load(int width, int height, int channels, u8 *data)
    {
        imageLoadPath = "";
        EnsureDestructionID();

        glGenTextures(1, &id);
        Bind();
        GLenum format = channels == 3 ? GL_RGB : GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        GLenum wrap = GL_REPEAT;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void Texture2D::Load(const std::string &name)
    {
        EnsureDestructionID();

        Image image = name;
        if (image.IsValid())
        {
            Load(image.GetWidth(), image.GetHeight(), image.GetChannels(), image.GetData());
            imageLoadPath = name;
        }
        else
        {
            CE_LOG("CE_RENDER", Error, "Unable to load image '%s' for texture, image invalid.",
                   name.c_str());
        }
    }

    void Texture2D::Bind() const { glBindTexture(GL_TEXTURE_2D, id); }

    void Texture2D::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

    void Texture2D::Destroy() { EnsureDestructionID(); }

    void Texture2D::Use(int index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        Bind();
    }

} // namespace Core
