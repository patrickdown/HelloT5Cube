#pragma once
#include <glad/gl.h>
#include <util/Wrapper.h>

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

	class VertexArray 
	{
		protected:
		GLuint handle = 0;

		public:

		explicit operator const GLuint& () { return handle; }

		void Create()
		{
			assert(handle == 0);
			glCreateVertexArrays(1, &handle);
		}

		void Delete()
		{
			if (handle != 0)
			{
				glDeleteVertexArrays(1, &handle);
				handle = 0;
			}
		}

		void Bind()
		{
			glBindVertexArray(handle);
		}

		public:

		void AttachBuffer(Buffer buffer, GLuint index, const VertexStructMember& desc);

	};

}