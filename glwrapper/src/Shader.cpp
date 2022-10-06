#include <glwrapper/Shader.h>
#include <util/Wrapper.h>

namespace GLWrapper
{

	bool Shader::Compile(GLenum shaderType, std::string code)
	{
		assert(handle == 0);
		handle = glCreateShader(shaderType);
		const char* ptrCode = code.c_str();
		glShaderSource(handle, 1, &ptrCode, NULL);
		glCompileShader(handle);

		return IsValid();
	}

	bool Shader::IsValid()
	{
		assert(handle != 0);
		int success;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

		return success != 0;
	}

	std::string Shader::GetErrorMessage()
	{
		char infoLog[512];
		glGetShaderInfoLog(handle, 512, NULL, infoLog);
		return std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog;
	}



	bool ShaderProgram::Compile(std::string vertexCode, std::string fragmentCode)
	{
		auto vertex = MakeOwned<Shader>();
		if (!vertex->Compile(GL_VERTEX_SHADER, vertexCode))
		{
			errorMsg = vertex->GetErrorMessage();
			return false;
		}

		auto fragment = MakeOwned<Shader>();
		if (!fragment->Compile(GL_FRAGMENT_SHADER, fragmentCode))
		{
			errorMsg = fragment->GetErrorMessage();
			return false;
		}

		assert(handle == 0);
		handle = glCreateProgram();

		glAttachShader(handle, (GLuint)*vertex);
		glAttachShader(handle, (GLuint)*fragment);
		glLinkProgram(handle);

		if (!IsValid())
		{
			char infoLog[512];
			glGetProgramInfoLog(handle, 512, NULL, infoLog);
			errorMsg = std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog;
			return false;
		}

		return true;
	}

	bool ShaderProgram::IsValid()
	{
		int success;
		glGetShaderiv(handle, GL_LINK_STATUS, &success);

		return success != 0;
	}
}
