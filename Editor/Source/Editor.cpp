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
        Editor() {};

        ~Editor() {};

        void Init()
        {
            CE_DEBUG("Application initialized");
            LayerStack::PushLayer(new EditorLayer());
        }

        void Update() {}

        void Shutdown() {}
    };
} // namespace Core

Core::Application *Core::CreateApplication() { return new Core::Editor(); }
