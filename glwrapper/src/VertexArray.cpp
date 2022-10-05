#include <glwrapper/VertexArray.h>
#include <glwrapper/Buffer.h>

namespace GLWrapper
{ 
	void VertexArray::AttachBuffer(Buffer& buffer, GLuint index, VertexStructMember& desc)
	{
		glEnableVertexArrayAttrib(Handle(), index);
		glVertexArrayVertexBuffer(Handle(), 0, buffer.Handle(), 0, desc.stride);
		glVertexArrayAttribBinding(Handle(), index, 0);
		glVertexArrayAttribFormat(Handle(), index, desc.size, desc.type, desc.normalized, desc.offset);
	}
}