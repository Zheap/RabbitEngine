#include "rbpch.h"
#include "OrthographicCameraController.h"

#include "Rabbit/Core/Input.h"
#include "Rabbit/COre/KeyCodes.h"

namespace Rabbit {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio),
          m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), 
          m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top),
          m_Ratation(rotation)
    {

    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        RB_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(RB_KEY_A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(RB_KEY_D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (Input::IsKeyPressed(RB_KEY_W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(RB_KEY_S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (m_Ratation)
        {
            if (Input::IsKeyPressed(RB_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(RB_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        RB_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(RB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(RB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::CaculateView()
    {
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        RB_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        CaculateView();
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        RB_PROFILE_FUNCTION();

        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        CaculateView();
        return false;
    }

}