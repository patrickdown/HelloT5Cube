#pragma once
#include <glwrapper/GLWrapper.h>

namespace GLWrapper {

	class Texture : public GLBindingWrapper<Texture>
	{
		protected:
		GLenum target;

		public:
		static bool CreateHandle(GLuint& handle, GLenum target)
		{
			glCreateTextures(target, 1, &handle);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}

		static void BindHandle(GLuint handle, GLuint unit)
		{
			glBindTextureUnit(unit, handle);
		}

		void SetWrapS(GLint wrap)
		{
			glTextureParameteri(Handle(), GL_TEXTURE_WRAP_S, wrap);
		}

		void SetWrapT(GLint wrap)
		{
			glTextureParameteri(Handle(), GL_TEXTURE_WRAP_T, wrap);
		}

		void SetMinFilter(GLint filter)
		{
			glTextureParameteri(Handle(), GL_TEXTURE_MIN_FILTER, filter);
		}

		void SetMagFilter(GLint filter)
		{
			glTextureParameteri(Handle(), GL_TEXTURE_MAG_FILTER, filter);
		}

		void SetTextureStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei levels = 1)
		{
			glTextureStorage2D(Handle(), levels, internalformat, width, height);
		}
	};


}