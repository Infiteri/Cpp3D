#include "CameraSystem.h"
#include "Core/Logger.h"

namespace Core
{
    static CameraSystem::State state;

    void CameraSystem::Init() { state.ActivePerspective = nullptr; }

    void CameraSystem::Shutdown() { state.Perspective.clear(); }

    void CameraSystem::AddPerspectiveCamera(const std::string &name,
                                            const PerspectiveCamera &camera)
    {
        if (state.Perspective.find(name) != state.Perspective.end())
        {
            CE_LOG("CE_RENDER", Warn, "Camera '%s' exists, nothind done.", name.c_str());
            return;
        }

        state.Perspective[name] = camera;
    }

    PerspectiveCamera *CameraSystem::GetPerspectiveCamera(const std::string &name)
    {
        if (state.Perspective.find(name) == state.Perspective.end())
            return nullptr;
        return &state.Perspective[name];
    }

    void CameraSystem::ActivatePerspectiveCamera(const std::string &name)
    {
        if (state.Perspective.find(name) == state.Perspective.end())
            return;

        state.ActivePerspective = &state.Perspective[name];
    }

    PerspectiveCamera *CameraSystem::GetActivePerspective() { return state.ActivePerspective; }
} // namespace Core
