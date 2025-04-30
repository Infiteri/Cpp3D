#pragma once

#include "Base.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Shader/Shader.h"

namespace Core
{
    struct GPUScreen
    {
        VertexArray *Array;
        Shader *Shader;
        FrameBuffer *Buffer;
        FrameBuffer *PostBuffer;

        void Create();
        void Destroy();

        void Begin();
        void End();
    };
} // namespace Core
