#pragma once

#include "Rabbit/Renderer/OrthographicCamera.h"
#include "Rabbit/Core/Timestep.h"
#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Events/MouseEvent.h"

namespace Rabbit {

    struct OrthographicCameraBounds
    {
        float Left, Right;
        float Bottom, Top;

        float GetWidth() { return Right - Left; }
        float GetHeight() { return Top - Bottom; }
    };

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool ratation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        void onResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; CaculateView(); }

        const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

    private:
        void CaculateView();

        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;

        OrthographicCameraBounds m_Bounds;
        OrthographicCamera m_Camera;

        bool m_Ratation;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 180.0f, m_CameraRotationSpeed = 180.0f;
    };

}