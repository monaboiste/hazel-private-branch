#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {
	
	Renderer::SceneData* Renderer::ms_sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		ms_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_viewProjection", ms_sceneData->viewProjectionMatrix);
		shader->UploadUniformMat4("u_transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}