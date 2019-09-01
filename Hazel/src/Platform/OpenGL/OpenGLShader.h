#pragma once

#include "Hazel\Renderer\Shader.h"
#include <glm\glm.hpp>

/* @TODO: remove */
typedef unsigned int GLenum;

namespace Hazel {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		int GetUniformLocation(const std::string& name) const;
	private:
		uint32_t m_rendererID;
		mutable std::unordered_map<std::string, int> m_cacheUniformLocation;
	};

}