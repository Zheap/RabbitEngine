#include "rbpch.h"
#include "Application.h"

#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Log.h"

namespace Rabbit
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategoryApplication))
        {
            RB_TRACE(e);
        }
        if (e.IsInCategory(EventCategoryInput))
        {
            RB_TRACE(e);
        }
        while (true);
    }
}