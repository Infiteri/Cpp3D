#include "Buffer.h"
#include "Core/Logger.h"

#include <glad/glad.h>

namespace Core
{
    Buffer::Buffer(Type type) : type(type)
    {
        id = 0;
        stride = 0;
        size = 0;

        glGenBuffers(1, &id);
        Bind();
    }

    Buffer::~Buffer() { Destroy(); }

    void Buffer::Bind()
    {
        glBindBuffer(((type == Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER), id);

        if (type == Vertex)
        {
            for (auto &lay : layout)
            {
                glEnableVertexAttribArray(lay.Location);
                glVertexAttribPointer(lay.Location, lay.Size, GL_FLOAT, GL_FALSE,
                                      stride * sizeof(float), (void *)(lay.Offset * sizeof(float)));
            }
        }
    }

    void Buffer::Unbind()
    {
        glBindBuffer((type == Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Buffer::Draw()
    {
        switch (type)
        {
        case Vertex:
            glDrawArrays(GL_TRIANGLES, 0,
                         size / (stride * 4)); // middle finger to the sizeof(float) i hate you
            break;

        case Index:
            glDrawElements(GL_TRIANGLES, size / 4, GL_UNSIGNED_INT, nullptr);
            break;
        }
    }

    void Buffer::UploadData(const void *data, u64 s)
    {
        Bind();

        this->size = s;

        switch (type)
        {
        case Vertex:
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            break;

        case Index:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            break;
        }
    }

    void Buffer::AddLayout(u32 loc, u32 off, u32 size)
    {
        if (type == Index)
            return;

        layout.push_back({loc, off, size});
        stride += size;
    }

    void Buffer::Destroy() { glDeleteBuffers(1, &id); }
} // namespace Core
