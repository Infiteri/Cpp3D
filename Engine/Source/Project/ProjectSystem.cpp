#include "ProjectSystem.h"
#include "Core/Logger.h"

namespace Core
{
    static ProjectSystem::State state;

    void ProjectSystem::Init()
    {
        CE_DEFINE_LOG_CATEGORY("CE_PROJECT", "Project");
        state.ActiveProject = nullptr;
    }

    void ProjectSystem::Shutdown()
    {
        if (state.ActiveProject)
            delete state.ActiveProject;
    }

    Project *ProjectSystem::GetActiveProject() { return state.ActiveProject; }

    void ProjectSystem::New()
    {
        if (state.ActiveProject)
            delete state.ActiveProject;

        state.ActiveProject = new Project();
    }

    void ProjectSystem::Load(const std::string &name)
    {
        if (state.ActiveProject)
            delete state.ActiveProject;

        state.ActiveProject = new Project();
        // todo : do here
    }
} // namespace Core
