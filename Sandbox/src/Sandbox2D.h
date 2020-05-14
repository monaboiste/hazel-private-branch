#pragma once

#include "ParticleSystem.h"

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
	Hazel::Ref<Hazel::Texture2D> m_bricksTexture;

	Hazel::Ref<Hazel::Texture2D> m_spriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_textureStairs;
	Hazel::Ref<Hazel::SubTexture2D> m_textureTree;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Hazel::OrthographicCameraController m_cameraController;

	ParticleProps m_particle;
	ParticleSystem m_particleSystem;

	uint32_t m_mapWidth, m_mapHeight;
	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> m_textureMap;
};