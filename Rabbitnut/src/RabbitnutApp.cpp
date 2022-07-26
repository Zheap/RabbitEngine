#include <Rabbit.h>
#include <Rabbit/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Rabbit {

    class Rabbitnut : public Application
    {
    public:
        Rabbitnut()
            : Application("Rabbitnut")
        {
            PushLayer(new EditorLayer());
        }
        ~Rabbitnut() {}
    };

    Application* CreateApplication()
    {
        return new Rabbitnut();
    }

}
