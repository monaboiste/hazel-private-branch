#include <Hazel.h>

#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Platform\OpenGL\OpenGLShader.h"
#include "Sandbox2D.h"

#include <chrono>


template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_name(name), m_func(func), m_stopped(false)
	{
		m_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		float duration = (end - start) * 0.001f;
		m_func({ m_name, duration });

		m_stopped = true;
	}
private:
	const char* m_name;
	Fn m_func;
	std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
	bool m_stopped;

};
#define PROFILE_SCOPE(name) Timer timer__LINE__(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_bricksTexture = Hazel::Texture2D::Create("assets/textures/bricks.jpg");
}

void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	PROFILE_SCOPE("Sanbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(ts);
	}
	{
		PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}
	{
		
		PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Hazel::Renderer2D::DrawQuad({ 0.5f, 0.8f }, { 1.0f, 0.8f }, m_squareColor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.8f, 2.2f }, m_bricksTexture);

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	bool showConsole = true;
	Hazel::ImGuiConsole::OnImGuiRender(&showConsole);
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));

	for (auto& result : m_profileResults)
	{
		char label[255];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);

		ImGui::Text(label, result.Time);
	}
	m_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
