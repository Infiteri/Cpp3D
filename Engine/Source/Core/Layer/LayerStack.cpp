#include "LayerStack.h"
#include <vector>

namespace Core
{
    static std::vector<Layer *> layers;

    // todo: default add of layers??
    void LayerStack::Init() {}

    void LayerStack::Shutdown()
    {
        for (auto layer : layers)
        {
            layer->OnDetach();
            delete layer;
        }

        layers.clear();
    }

    void LayerStack::Update()
    {
        for (auto l : layers)
            l->OnUpdate();
    }

    void LayerStack::RenderImGui()
    {
        for (auto l : layers)
            l->OnImGuiRender();
    }

    void LayerStack::EmitEvent(Event *event)
    {
        CE_VERIFY(event);

        for (auto l : layers)
            l->OnEvent(event);
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        layers.push_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        // todo: maybe not by address direct comparison
        for (u64 i = 0; i < layers.size(); i++)
        {
            if (layers[i] == layer)
            {
                layers[i]->OnDetach();
                delete layers[i];
                layers.erase(layers.begin() + i);
            }
        }
    }
} // namespace Core
