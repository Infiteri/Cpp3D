#pragma once

#include "Base.h"
#include "Renderer/GPUScreen.h"
#include "Renderer/Object/Mesh.h"

namespace Core
{
    struct ViewportState
    {
        int Width;
        int Height;
        inline float GetAspect() { return (float)Width / (float)Height; };
    };

    class Renderer
    {
    public:
        struct State
        {
            bool HasContext = false;
            ViewportState ScreenViewport;
            GPUScreen Screen;
        };

    public:
        Renderer() {};
        ~Renderer() {};

        static void InitializeGLAD();

        static void Init();
        static void Shutdown();

        static void Viewport(int width, int height);
        static ViewportState &GetViewport();

        static void SetColor(const Color &color);
        static CE_API Color &GetColor();
        static CE_API Mesh *GetMesh();

        static void BeginFrame();
        static void Render();
        static void EndFrame();
    };
} // namespace Core
