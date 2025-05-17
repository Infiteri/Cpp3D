#pragma once

#include "Renderer/Shader/Shader.h"

namespace Core
{
    class Light
    {
    public:
        Light() {};
        virtual ~Light() {};

        virtual void Render(Shader *shader) {};
    };

    class LightIDManager
    {
    public:
        struct State
        {
            u32 PointLightID = 0;
            u32 PointLastFrameCount = 0;

            u32 SpotLightID = 0;
            u32 SpotLastFrameCount = 0;
        };

    public:
        LightIDManager() {};
        ~LightIDManager() {};

        static void BeginFrame();

        static u32 PointGetLastFrameCount();
        static u32 PointGetID();

        static u32 SpotGetLastFrameCount();
        static u32 SpotGetID();
    };
} // namespace Core
