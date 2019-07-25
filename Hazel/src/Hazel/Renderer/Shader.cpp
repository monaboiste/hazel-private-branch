#include "hzpch.h"
#include "Shader.h"

#include "Hazel\Log.h"

#include <glad\glad.h>
#include <glm\gtc\type_ptr.hpp>

namespace Hazel {

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar *source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader commpilation failure!");

			glDeleteShader(vertexShader);

			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Fragment shader commpilation failure!");

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			return;
		}

		m_rendererID = glCreateProgram();

		glAttachShader(m_rendererID, vertexShader);
		glAttachShader(m_rendererID, fragmentShader);

		glLinkProgram(m_rendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");

			glDeleteProgram(m_rendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(m_rendererID, vertexShader);
		glDetachShader(m_rendererID, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}
