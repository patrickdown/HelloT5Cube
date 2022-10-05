#pragma once
#include <glwrapper/GLWrapper.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

namespace GLWrapper
{
	class Shader : public GLWrapperBase<Shader>
	{
		public:
		static bool CreateHandle(GLuint& handle, GLenum shaderType)
		{
			handle = glCreateShader(shaderType);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteShader(handle);
			handle = 0;
		}

		bool Compile(std::string code);
		bool IsValid();
		std::string GetErrorMessage();
	};

	class ShaderProgram : public GLBindingWrapper<ShaderProgram>
	{
		public:
		static bool CreateHandle(GLuint& handle)
		{
			handle = glCreateProgram();
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteProgram(handle);
		}

		static void BindHandle(GLuint handle)
		{
			glUseProgram(handle);
		}

		public:

		std::string errorMsg;

		inline void Use() { Bind(); }

		GLint GetAttribLocation(std::string name)
		{
			return glGetAttribLocation(Handle(), name.c_str());
		}

		void Set(std::string name, glm::mat4x4& value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		}

		void Set(std::string name, float value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform1f(loc, value);
		}

		void Set(std::string name, glm::vec2& value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform2fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, glm::vec3& value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform3fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, glm::vec4& value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform4fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, GLint value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform1i(loc, value);
		}

		void Set(std::string name, GLuint value)
		{
			auto loc = glGetUniformLocation(Handle(), name.c_str());
			glUniform1ui(loc, value);
		}

		bool Compile(std::string vertexCode, std::string fragmentCode);
		bool IsValid();
		std::string GetErrorMessage() { return errorMsg; }
	};
}


