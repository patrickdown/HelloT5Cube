#pragma once
#include <string>
#include <glad/gl.h>
#include <glwrapper/GLWrapper.h>
#include <glwrapper/Texture.h>
#include <glwrapper/Renderbuffer.h>

namespace GLWrapper
{
	

	class Framebuffer : public GLBindingWrapper<Framebuffer>
	{
		public:
		static bool CreateHandle(GLuint& handle)
		{
			glCreateFramebuffers(1, &handle);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteFramebuffers(1, &handle);
			handle = 0;
		}

		static void BindHandle(GLuint handle, GLenum target)
		{
			glBindFramebuffer(target, handle);
		}

		void AttachColor(Texture& texture, int textureLevel = 0, int colorAttachment = 0)
		{
			glNamedFramebufferTexture(Handle(), GL_COLOR_ATTACHMENT0 + colorAttachment, texture.Handle(), textureLevel);
		}

		void AttachColor(Renderbuffer& renderbuffer, int colorAttachment = 0)
		{
			glNamedFramebufferRenderbuffer(Handle(), GL_COLOR_ATTACHMENT0 + colorAttachment, GL_RENDERBUFFER, renderbuffer.Handle());
		}

		void AttachDepth(Texture& texture, int textureLevel = 0)
		{
			glNamedFramebufferTexture(Handle(), GL_DEPTH_ATTACHMENT, texture.Handle(), textureLevel);
		}

		void AttachDepth(Renderbuffer& renderbuffer)
		{
			glNamedFramebufferRenderbuffer(Handle(), GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.Handle());
		}

		void AttachStencil(Texture& texture, int textureLevel = 0)
		{
			glNamedFramebufferTexture(Handle(), GL_STENCIL_ATTACHMENT, texture.Handle(), textureLevel);
		}

		void DrawBuffers(int colorAttachment)
		{
			
			GLenum attach = GL_COLOR_ATTACHMENT0 + colorAttachment;
			glNamedFramebufferDrawBuffers(Handle(), 1, &attach);
		}


		void AttachStencil(Renderbuffer& renderbuffer)
		{
			glNamedFramebufferRenderbuffer(Handle(), GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.Handle());
		}

		void BlitToScreen(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
		{
			glBlitNamedFramebuffer(Handle(), 0, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
		}

		GLenum CheckStatus()
		{
			return glCheckNamedFramebufferStatus(Handle(), GL_FRAMEBUFFER);
		}

		bool Ready()
		{
			return CheckStatus() == GL_FRAMEBUFFER_COMPLETE;
		}

		std::string GetErrorMessage();
	
	};

}

