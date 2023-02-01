#pragma once
#include <util/Wrapper.h>

namespace GLWrapper {

	class Texture 
	{
		protected:
		GLuint handle = 0;

		public:
		explicit operator GLuint () { return handle; }

		void* HandleAsVoidPtr()
		{
			return reinterpret_cast<void*>(static_cast<uintptr_t>(handle));
		}

		void Create(GLenum target)
		{
			assert(handle == 0);
			glCreateTextures(target, 1, &handle);
		}

		void Delete()
		{
			if (handle != 0)
			{
				glDeleteTextures(1, &handle);
				handle = 0;
			}
		}

		static void BindHandle(GLuint handle, GLuint unit)
		{
			assert(handle != 0);
			glBindTextureUnit(unit, handle);
		}

		void SetWrapS(GLint wrap)
		{
			assert(handle != 0);
			glTextureParameteri(handle, GL_TEXTURE_WRAP_S, wrap);
		}

		void SetWrapT(GLint wrap)
		{
			assert(handle != 0);
			glTextureParameteri(handle, GL_TEXTURE_WRAP_T, wrap);
		}

		void SetMinFilter(GLint filter)
		{
			assert(handle != 0);
			glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, filter);
		}

		void SetMagFilter(GLint filter)
		{
			assert(handle != 0);
			glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, filter);
		}

		void SetTextureStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei levels = 1)
		{
			assert(handle != 0);
			glTextureStorage2D(handle, levels, internalformat, width, height);
		}

		void SetTextureStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei levels = 1) 
		{
			assert(handle != 0);
			glTextureStorage3D(handle, levels, internalformat, width, height, depth);

		}
	};


}