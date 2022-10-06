#pragma once
#include <glad/gl.h>
#include <util/Wrapper.h>

namespace GLWrapper
{

	class Renderbuffer
	{
		protected:
		GLuint handle = 0;

		public:

		explicit operator const GLuint& () { return handle; }

		public:
		void Create()
		{
			assert(handle == 0);
			glCreateRenderbuffers(1, &handle);
		}

		void Delete()
		{
			if (handle != 0)
			{
				glDeleteRenderbuffers(1, &handle);
				handle = 0;
			}
		}

		void Bind(GLenum target)
		{
			glBindRenderbuffer(target, handle);
		}

		void SetDepthStorage(GLsizei width, GLsizei height)
		{
			assert(handle != 0);
			glNamedRenderbufferStorage(handle, GL_DEPTH_COMPONENT, width, height);
		}

		void SetStencilStorage(GLsizei width, GLsizei height)
		{
			assert(handle != 0);
			glNamedRenderbufferStorage(handle, GL_STENCIL_COMPONENTS, width, height);
		}

		void SetColorStorage(GLenum format, GLsizei width, GLsizei height)
		{
			assert(handle != 0);
			glNamedRenderbufferStorage(handle, format, width, height);
		}
	};
}

