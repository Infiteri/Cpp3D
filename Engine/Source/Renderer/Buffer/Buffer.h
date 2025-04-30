#pragma once

#include "Base.h"
#include <vector>

namespace Core
{
    class Buffer
    {

    public:
        struct Layout
        {
            u32 Location;
            u32 Offset;
            u32 Size;
        };

        enum Type
        {
            Vertex,
            Index
        };

    private:
        u32 id;
        u32 stride;
        u32 size;
        Type type;

        std::vector<Layout> layout;

    public:
        Buffer(Type type = Vertex);
        ~Buffer();

        void Bind();
        void Unbind();
        void Draw();
        void Destroy();
        void UploadData(const void *data, u64 size);

        /// @brief Matters only for Vertex buffers. Nothing happens on Index buffer/.
        void AddLayout(u32 loc, u32 off, u32 size);

        inline u32 GetID() { return id; };
    };
} // namespace Core
