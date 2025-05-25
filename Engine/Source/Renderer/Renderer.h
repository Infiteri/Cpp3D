#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "Renderer/GPUScreen.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Object/Sky.h"
#include "Renderer/Object/Skybox.h"
#include "Renderer/PostProcessor.h"
#include "Renderer/Texture/CubemapTexture.h"

namespace Core
{
    struct ViewportState
    {
        int Width;
        int Height;
        inline float GetAspect() { return (float)Width / (float)Height; };
    };

    class CE_API Renderer
    {
    public:
        struct State
        {
            bool HasContext = false;
            ViewportState ScreenViewport;
            GPUScreen Screen;

            Skybox Skybox;

            Sky *SkyInstance;
            DirectionalLight *DirectionalLightInstance;

            PostProcessor Post, SkyPost;
        };

    public:
        Renderer() {};
        ~Renderer() {};

        static void InitializeGLAD();

        static void Init();
        static void Shutdown();

        static CE_API void Viewport(int width, int height);
        static ViewportState &GetViewport();

        static void SetSkyInstance(Sky *sky);
        static void SetDirectioanlLightInstance(DirectionalLight *light);

        static void ReloadPostProcessShaders();

        static void BeginFrame();
        static void Render();
        static void EndFrame();

        static void RenderCubemapShader(Shader *shader);
        static void RenderCubemapTexture(CubemapTexture *texture);
        static void RenderCubemapTexture(CubemapTexture *texture, VertexArray *array,
                                         Shader *shader, PerspectiveCamera *camera);

        static void UploadCameraToShader(Shader *shader, PerspectiveCamera *camera);

        static CE_API u32 GetSceneViewportPassID();
    };
} // namespace Core
