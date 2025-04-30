#include "VertexArray.h"

#include <glad/glad.h>

namespace Core
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
        Bind();

        vertexBuffer = nullptr;
        indexBuffer = nullptr;
    }

    VertexArray::~VertexArray()
    {
        Destroy();

        if (vertexBuffer)
            delete vertexBuffer;

        if (indexBuffer)
            delete indexBuffer;
    }

    void VertexArray::Bind() { glBindVertexArray(id); }

    void VertexArray::Unbind() { glBindVertexArray(0); }

    void VertexArray::Destroy() { glDeleteVertexArrays(1, &id); }

    void VertexArray::GenerateVertexBuffer(const void *data, u32 size)
    {
        if (vertexBuffer)
            delete vertexBuffer;

        vertexBuffer = new Buffer(Buffer::Vertex);
        vertexBuffer->UploadData(data, size);
    }

    void VertexArray::GenerateIndexBuffer(const void *data, u32 size)
    {
        if (indexBuffer)
            delete indexBuffer;

        indexBuffer = new Buffer(Buffer::Index);
        indexBuffer->UploadData(data, size);
    }
} // namespace Core
