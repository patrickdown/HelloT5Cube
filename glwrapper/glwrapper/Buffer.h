#pragma once
#include <memory>
#include <vector>
#include <span>
#include "GLWrapper.h"

namespace GLWrapper 
{
	
	class Buffer : public GLWrapperBase<Buffer>
	{
		public:

		static bool CreateHandle(GLuint& handle)
		{
			glCreateBuffers(1, &handle);
			return true;
		}

		static void DeleteHandle(GLuint& handle)
		{
			glDeleteBuffers(1, &handle);
			handle = 0;
		}

		protected:
		size_t elementSize;

		public:

		template<typename T>
		void StoreData(std::span<T> vertices, GLbitfield  flags = 0)
		{
			elementSize = sizeof(T);
			glNamedBufferStorage(Handle(), vertices.size_bytes(), vertices.data(), flags);
		}

		size_t GetElementSize() 
		{ 
			return elementSize; 
		}
	};

}