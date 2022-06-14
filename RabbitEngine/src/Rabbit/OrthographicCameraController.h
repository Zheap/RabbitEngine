#pragma once

#include "Rabbit/Renderer/OrthographicCamera.h"
#include "Rabbit/Core/Timestep.h"
#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Events/MouseEvent.h"

namespace Rabbit {

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool ratation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_Ratation;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 180.0f, m_CameraRotationSpeed = 1.0f;
    };

}