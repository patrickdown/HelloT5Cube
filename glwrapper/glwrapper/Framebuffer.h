#pragma once
#include <string>
#include <glad/gl.h>
#include <util/Wrapper.h>
#include <glwrapper/Texture.h>
#include <glwrapper/Renderbuffer.h>

namespace GLWrapper
{
	

	class Framebuffer 
	{
		protected:
		GLuint handle = 0;

		public:
		explicit operator GLuint () { return handle; }

		void Create()
		{
			assert(handle == 0);
			glCreateFramebuffers(1, &handle);
		}

		void Delete()
		{
			if (handle != 0)
			{
				glDeleteFramebuffers(1, &handle);
				handle = 0;
			}
		}

		void Bind(GLenum target)
		{
			glBindFramebuffer(target, handle);
		}

		void ColorAttachment(Texture texture, int textureLevel = 0, int colorAttachment = 0)
		{
			assert(handle != 0);
			glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT0 + colorAttachment, (GLuint)texture, textureLevel);
		}

		void ColorAttachment(Renderbuffer renderbuffer, int colorAttachment = 0)
		{
			assert(handle != 0);
			glNamedFramebufferRenderbuffer(handle, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_RENDERBUFFER, (GLuint)renderbuffer);
		}

		void DepthAttachment(Texture texture, int textureLevel = 0)
		{
			assert(handle != 0);
			glNamedFramebufferTexture(handle, GL_DEPTH_ATTACHMENT, (GLuint)texture, textureLevel);
		}

		void DepthAttachment(Renderbuffer renderbuffer)
		{
			assert(handle != 0);
			glNamedFramebufferRenderbuffer(handle, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, (GLuint)renderbuffer);
		}

		void StencilAttachment(Texture texture, int textureLevel = 0)
		{
			assert(handle != 0);
			glNamedFramebufferTexture(handle, GL_STENCIL_ATTACHMENT, (GLuint)texture, textureLevel);
		}


		void StencilAttachment(Renderbuffer renderbuffer)
		{
			assert(handle != 0);
			glNamedFramebufferRenderbuffer(handle, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, (GLuint)renderbuffer);
		}

		void DrawBuffers(int colorAttachment)
		{
			assert(handle != 0);
			GLenum attach = GL_COLOR_ATTACHMENT0 + colorAttachment;
			glNamedFramebufferDrawBuffers(handle, 1, &attach);
		}

		void BlitToScreen(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
		{
			assert(handle != 0);
			glBlitNamedFramebuffer(handle, 0, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
		}

		GLenum CheckStatus()
		{
			assert(handle != 0);
			return glCheckNamedFramebufferStatus(handle, GL_FRAMEBUFFER);
		}

		bool IsReady()
		{
			if(handle != 0)
				return CheckStatus() == GL_FRAMEBUFFER_COMPLETE;
			return false;
		}

		std::string GetErrorMessage();
	
	};

}

