#include <glwrapper/VertexArray.h>
#include <glwrapper/Buffer.h>

namespace GLWrapper
{ 
	void VertexArray::AttachBuffer(Buffer buffer, GLuint index, const VertexStructMember& desc)
	{
		assert(handle != 0);
		glEnableVertexArrayAttrib(handle, index);
		glVertexArrayVertexBuffer(handle, 0, (GLuint)buffer, 0, desc.stride);
		glVertexArrayAttribBinding(handle, index, 0);
		glVertexArrayAttribFormat(handle, index, desc.size, desc.type, desc.normalized, desc.offset);
	}
}