#pragma once
#include <memory>
#include <vector>
#include <span>
#include <util/Wrapper.h>

namespace GLWrapper 
{
	

	class Buffer 
	{
		protected:
		GLuint handle = 0;

		public:

		explicit operator const GLuint&() { return handle; }

		void Create()
		{
			assert(handle == 0);
			glCreateBuffers(1, &handle);
		}

		void Delete()
		{
			if (handle != 0)
			{
				glDeleteBuffers(1, &handle);
				handle = 0;
			}
		}

		template<typename T>
		void StoreData(std::span<T> vertices, GLbitfield  flags = 0)
		{
			assert(handle != 0);
			glNamedBufferStorage(handle, vertices.size_bytes(), vertices.data(), flags);
		}
	};


}