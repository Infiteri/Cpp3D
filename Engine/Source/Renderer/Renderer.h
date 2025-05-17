#pragma once

#include "Base.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/GPUScreen.h"

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

        static CE_API void Viewport(int width, int height);
        static ViewportState &GetViewport();

        static void SetColor(const Color &color);
        static CE_API Color &GetColor();

        static void BeginFrame();
        static void Render();
        static void EndFrame();

        static void UploadCameraToShader(Shader *shader, PerspectiveCamera *camera);

        static CE_API u32 GetSceneViewportPassID();
    };
} // namespace Core
