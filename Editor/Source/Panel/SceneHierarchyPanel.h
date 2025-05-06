#pragma once

#include "Panel.h"
#include "Scene/Actor.h"

namespace Core
{
    class SceneHierarchyPanel : public Panel
    {
    private:
        Actor *selected = nullptr;

    public:
        SceneHierarchyPanel() {};
        ~SceneHierarchyPanel() {};

        inline Actor *GetSelectedActor() { return selected; };

        void RenderActor(Actor *a, Actor *parent = nullptr, bool parentNodeOpen = false);
        void RenderActorComponents(Actor *a);

        void OnImGuiRender();
    };
} // namespace Core
