#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Core
{
    Image::Image(const std::string &name) { Load(name); }

    Image::~Image() { DestroyData(); }

    void Image::Load(const std::string &name)
    {
        this->name = name;
        isValid = true;
        data = stbi_load(name.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            isValid = false;
            data = 0;
            width = 0;
            height = 0;
            this->name = "";
        }
    }

    void Image::DestroyData()
    {
        if (data)
            stbi_image_free(data);

        data = 0;
    }

} // namespace Core
