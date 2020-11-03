#include "hzpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {
	
	Scope<Renderer::SceneData> Renderer::ms_sceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		/* @TODO: Rework this for multiple framebuffers */
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		ms_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_viewProjection", ms_sceneData->viewProjectionMatrix);
		shader->SetMat4("u_transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}