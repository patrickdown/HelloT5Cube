#pragma once
#include <util/Wrapper.h>
#include <iostream>

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

		void Generate() {
			assert(handle == 0);
			glGenTextures(1, &handle);
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

		void SetTextureStorage(GLenum internalformat, GLsizei width, GLsizei height)
		{
			assert(handle != 0);
			glTextureStorage2D(handle, 1, internalformat, width, height);
		}

		void SetTextureStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) 
		{
			assert(handle != 0);
			glTextureStorage3D(handle, 1, internalformat, width, height, depth);
		}

		void SetTextureMipmapStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei levels) {
			assert(handle != 0);
			glTextureStorage2D(handle, levels, internalformat, width, height);
		}

		void SetTextureMipmapStorage(GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei levels) {
			assert(handle != 0);
			glTextureStorage3D(handle, levels, internalformat, width, height, depth);
		}

		void SetDepthStorage(GLsizei width, GLsizei height) {
			assert(handle != 0);
			glTextureStorage2D(handle, 1, GL_DEPTH_COMPONENT24, width, height);
		}

		void SetDepthStorage( GLsizei width, GLsizei height, GLsizei depth) {
			assert(handle != 0);
			glTextureStorage3D(handle, 1, GL_DEPTH_COMPONENT24, width, height, depth);

		}

		Owned<Texture> CreateView(GLenum target,
						GLenum internalformat,
						GLuint minlevel,
						GLuint numlevels,
						GLuint minlayer,
						GLuint numlayers) {

			Owned<Texture> view = MakeOwned<Texture>();
			view->Generate();
			glTextureView((GLuint)*view, target, handle, internalformat, minlevel, numlevels, minlayer, numlayers);

			return view;
		}
	};


}