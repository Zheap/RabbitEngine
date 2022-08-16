#pragma once

#include "Rabbit.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Rabbit/Renderer/EditorCamera.h"

namespace Rabbit {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& e) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

    private:
        OrthographicCameraController m_CameraController;

        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;

        Entity m_HoveredEntity;

        bool m_PrimaryCamera = true;

        EditorCamera m_EditorCamera;

        Ref<Texture2D> m_CheckedboardTexture;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

        int m_GizmoType = -1;

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };

}
