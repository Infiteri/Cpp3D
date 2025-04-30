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
            layer->OnDestroy();
            delete layer;
        }

        layers.clear();
    }

    void LayerStack::OnUpdate()
    {
        for (auto l : layers)
            l->OnUpdate();
    }

    void LayerStack::OnImGuiRender()
    {
        for (auto l : layers)
            l->OnImGuiRender();
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        layers.push_back(layer);
        layer->OnInit();
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        // todo: maybe not by address direct comparison
        for (u64 i = 0; i < layers.size(); i++)
        {
            if (layers[i] == layer)
            {
                layers[i]->OnDestroy();
                delete layers[i];
                layers.erase(layers.begin() + i);
            }
        }
    }
} // namespace Core
