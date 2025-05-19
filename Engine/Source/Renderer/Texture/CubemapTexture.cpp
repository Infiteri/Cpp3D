#include "CubemapTexture.h"
#include "Core/Logger.h"
#include "Resource/Image.h"

#include <glad/glad.h>

namespace Core
{

    void CubemapTexture::_EnsureDestructionID()
    {
        if (id != 0)
            glDeleteTextures(1, &id);
        id = 0;
    }

    void CubemapTexture::Load(u32 side, int w, int h, int channel, u8 *data)
    {
        // todo: Channels
        glTexImage2D(side, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    CubemapTexture::CubemapTexture() {}

    CubemapTexture::~CubemapTexture() { Destroy(); }

    void CubemapTexture::Load()
    {
        _EnsureDestructionID();

        glGenTextures(1, &id);
        Bind();

        u8 data[] = {255, 255, 255};
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 1, 1, 3, data);
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 1, 1, 3, data);
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 1, 1, 3, data);
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 1, 1, 3, data);
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 1, 1, 3, data);
        Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 1, 1, 3, data);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void CubemapTexture::Load(CubemapConfiguration &config)
    {
        _EnsureDestructionID();

        glGenTextures(1, &id);
        Bind();

        state = config;

        std::string path[] = {config.Right,  config.Left,  config.Top,
                              config.Bottom, config.Front, config.Back};
        for (int i = 0; i < 6; i++)
        {
            Image img = Image(path[i]); // todo: does this malloc more that it should?
            Load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, img.GetWidth(), img.GetHeight(),
                 img.GetChannels(), img.GetData());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void CubemapTexture::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

    void CubemapTexture::Unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

    void CubemapTexture::Destroy() { _EnsureDestructionID(); }

    void CubemapTexture::Use()
    {
        glActiveTexture(GL_TEXTURE0);
        Bind();
    }

} // namespace Core
