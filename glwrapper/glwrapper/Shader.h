#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

namespace GLWrapper
{
	class Shader 
	{
		protected:
		GLuint handle = 0;

		public:
		explicit operator const GLuint& () { return handle; }

		void Delete()
		{
			glDeleteShader(handle);
			handle = 0;
		}

		bool Compile(GLenum shaderType, std::string code);
		bool IsValid();
		std::string GetErrorMessage();
	};

	class ShaderProgram
	{
		protected:
		GLuint handle = 0;
		std::string errorMsg;

		public:
		explicit operator const GLuint& () { return handle; }

		void Delete()
		{
			if(handle != 0)
				glDeleteProgram(handle);
		}

		void Use() 
		{
			assert(handle != 0);
			glUseProgram(handle);
		}

		GLint GetAttribLocation(std::string name)
		{
			assert(handle != 0);
			return glGetAttribLocation(handle, name.c_str());
		}

		void Set(std::string name, glm::mat4x4& value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		}

		void Set(std::string name, float value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform1f(loc, value);
		}

		void Set(std::string name, glm::vec2& value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform2fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, glm::vec3& value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform3fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, glm::vec4& value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform4fv(loc, 1, glm::value_ptr(value));
		}

		void Set(std::string name, GLint value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform1i(loc, value);
		}

		void Set(std::string name, GLuint value)
		{
			assert(handle != 0);
			auto loc = glGetUniformLocation(handle, name.c_str());
			glUniform1ui(loc, value);
		}

		bool Compile(std::string vertexCode, std::string fragmentCode);
		bool IsValid();
		std::string GetErrorMessage() { return errorMsg; }
	};
}


