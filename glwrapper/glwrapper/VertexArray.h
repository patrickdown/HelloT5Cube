#pragma once
#include <glwrapper/GLWrapper.h>

namespace GLWrapper
{
	class Buffer;

	struct VertexStructMember
	{
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		GLuint offset;
	};

	class VertexArray : public GLBindingWrapper<VertexArray>
	{
		public:
		static bool CreateHandle(GLuint& handle)
		{
			glCreateVertexArrays(1, &handle);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteVertexArrays(1, &handle);
			handle = 0;
		}

		static void BindHandle(GLuint handle)
		{
			glBindVertexArray(handle);
		}

		public:

		void AttachBuffer(Buffer& buffer, GLuint index, VertexStructMember& desc);

	};

}