#include <glwrapper/Shader.h>

namespace GLWrapper
{

	bool Shader::Compile(std::string code)
	{
		const char* ptrCode = code.c_str();
		glShaderSource(Handle(), 1, &ptrCode, NULL);
		glCompileShader(Handle());

		return IsValid();
	}

	bool Shader::IsValid()
	{
		int success;
		glGetShaderiv(Handle(), GL_COMPILE_STATUS, &success);

		return success != 0;
	}

	std::string Shader::GetErrorMessage()
	{
		char infoLog[512];
		glGetShaderInfoLog(Handle(), 512, NULL, infoLog);
		return std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog;
	}



	bool ShaderProgram::Compile(std::string vertexCode, std::string fragmentCode)
	{
		auto vertex = Shader::Make(GL_VERTEX_SHADER);
		if (!vertex.Compile(vertexCode))
		{
			errorMsg = vertex.GetErrorMessage();
			return false;
		}

		auto fragment = Shader::Make(GL_FRAGMENT_SHADER);
		if (!fragment.Compile(fragmentCode))
		{
			errorMsg = fragment.GetErrorMessage();
			return false;
		}

		glAttachShader(Handle(), vertex.Handle());
		glAttachShader(Handle(), fragment.Handle());
		glLinkProgram(Handle());

		if (!IsValid())
		{
			char infoLog[512];
			glGetProgramInfoLog(Handle(), 512, NULL, infoLog);
			errorMsg = std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog;
			return false;
		}

		return true;
	}

	bool ShaderProgram::IsValid()
	{
		int success;
		glGetShaderiv(Handle(), GL_LINK_STATUS, &success);

		return success != 0;
	}
}
