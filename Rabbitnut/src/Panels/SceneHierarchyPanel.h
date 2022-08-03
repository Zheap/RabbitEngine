#pragma once

#include "Rabbit/Core/Core.h"
#include "Rabbit/Core/Log.h"
#include "Rabbit/Scene/Scene.h"
#include "Rabbit/Scene/Entity.h"
#include "Rabbit/Scene/Component.h"

namespace Rabbit {

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);
    private:
        Ref<Scene> m_Context;

        Entity m_SelectionContext;
    };

}