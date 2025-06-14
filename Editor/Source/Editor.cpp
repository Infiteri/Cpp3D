#include "Core/Application.h"
#include "Core/Launch/EntryPoint.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Logger.h"
#include "EditorLayer.h"

namespace Core
{
    class Editor : public Application
    {
    public:
        ApplicationArguments args;

        Editor(ApplicationArguments &inArgs) { args = inArgs; };

        void Init() { LayerStack::PushLayer(new EditorLayer(args)); }
    };
} // namespace Core

Core::Application *Core::CreateApplication(Core::ApplicationArguments &args)
{
    return new Core::Editor(args);
}
