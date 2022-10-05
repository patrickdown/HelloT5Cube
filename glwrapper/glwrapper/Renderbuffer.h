#pragma once
#include <glad/gl.h>
#include <glwrapper/GLWrapper.h>


namespace GLWrapper
{

	class Renderbuffer : public GLBindingWrapper<Renderbuffer>
	{
		public:
		static bool CreateHandle(GLuint& handle)
		{
			glCreateRenderbuffers(1, &handle);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteRenderbuffers(1, &handle);
			handle = 0;
		}

		static void BindHandle(GLuint handle, GLenum target)
		{
			glBindRenderbuffer(target, handle);
		}

		void SetDepthStorage(GLsizei width, GLsizei height)
		{
			glNamedRenderbufferStorage(Handle(), GL_DEPTH_COMPONENT, width, height);
		}

		void SetStencilStorage(GLsizei width, GLsizei height)
		{
			glNamedRenderbufferStorage(Handle(), GL_STENCIL_COMPONENTS, width, height);
		}

		void SetColorStorage(GLenum format, GLsizei width, GLsizei height)
		{
			glNamedRenderbufferStorage(Handle(), format, width, height);
		}
	};
}

