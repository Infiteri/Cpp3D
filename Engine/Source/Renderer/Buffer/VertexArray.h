#pragma once

#include "Base.h"
#include "Buffer.h"

namespace Core
{
    class VertexArray
    {
    private:
        u32 id;

        Buffer *vertexBuffer;
        Buffer *indexBuffer;

    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();
        void Destroy();

        void GenerateVertexBuffer(const void *data, u32 size);
        void GenerateIndexBuffer(const void *data, u32 size);

        inline Buffer *GetVertexBuffer() { return vertexBuffer; };
        inline Buffer *GetIndexBuffer() { return indexBuffer; };
        inline u32 GetID() { return id; };
    };
} // namespace Core
