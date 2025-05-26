#pragma once

#include "Base.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class CE_API CameraSystem
    {

    public:
        struct State
        {
            std::unordered_map<std::string, PerspectiveCamera> Perspective;
            PerspectiveCamera *ActivePerspective = nullptr;
        };

    public:
        CameraSystem() {};
        ~CameraSystem() {};

        static void Init();
        static void Shutdown();

        static void AddPerspectiveCamera(const std::string &name, const PerspectiveCamera &camera);
        static PerspectiveCamera *GetPerspectiveCamera(const std::string &name);

        static void ActivatePerspectiveCamera(const std::string &name);
        static void ActivatePerspectiveCameraInstance(PerspectiveCamera *camera);
        static PerspectiveCamera *GetActivePerspective();
    };
} // namespace Core
