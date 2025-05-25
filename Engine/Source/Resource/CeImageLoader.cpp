#include "CeImageLoader.h"
#include "Core/Logger.h"

#include <fstream>
#include <stb_image.h>

namespace Core
{
    struct ImageData
    {
        int Width;
        int Height;
        int Channels;
        u8 *Data;
    };

    void CeImageLoader::CreateCeImage(const std::string &input, const std::string &output)
    {
        if (input.empty() || output.empty())
        {
            CE_ERROR("CeImageLoader: Cannot create image, input or output is empty");
            return;
        }

        ImageData data;
        data.Data = stbi_load(input.c_str(), &data.Width, &data.Height, &data.Channels, 0);

        if (!data.Data)
        {
            CE_ERROR("CeImageLoader: Image '%s' is not valid.", input.c_str());
            return;
        }

        std::ofstream stream(output, std::ios::binary);
        stream.write((const char *)(&data.Width), sizeof(int));
        stream.write((const char *)(&data.Height), sizeof(int));
        stream.write((const char *)(&data.Channels), sizeof(int));
        stream.write((const char *)data.Data, data.Width * data.Height * data.Channels);
        stream.close();
    }

    char *CeImageLoader::LoadCeImage(const std::string &input, int *width, int *height,
                                     int *channels)
    {
        std::ifstream file(input, std::ios::binary);
        if (!file)
        {
            CE_ERROR("CeImageLoader: Unable to load '%s'", input.c_str());
            return nullptr;
        }

        ImageData imgData;
        file.read((char *)(&imgData.Width), sizeof(int));
        file.read((char *)(&imgData.Height), sizeof(int));
        file.read((char *)(&imgData.Channels), sizeof(int));
        *width = imgData.Width;
        *height = imgData.Height;
        *channels = imgData.Channels;

        u32 imgSize = imgData.Width * imgData.Height * imgData.Channels;
        char *data = new char[imgSize];
        file.read((char *)data, imgSize);
        file.close();
        return data;
    }
} // namespace Core
