#pragma once

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;
private:
	Hazel::Ref<Hazel::VertexArray> m_squareVA;
	Hazel::Ref<Hazel::Shader> m_flatColorShader;

	Hazel::OrthographicCameraController m_cameraController;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};