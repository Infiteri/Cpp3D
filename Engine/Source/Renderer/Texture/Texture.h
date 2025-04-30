#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Texture
    {
    protected:
        u32 id;

    public:
        Texture() {};
        virtual ~Texture() {};

        virtual void Load() = 0;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Destroy() = 0;

        inline u32 GetID() { return id; };
    };
} // namespace Core
