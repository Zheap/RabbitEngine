#pragma once

#include "Rabbit/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Rabbit {

    class RABBIT_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverLay(Layer* overLay);
        void PopLayer(Layer* layer);
        void PopOverLay(Layer* overLay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}