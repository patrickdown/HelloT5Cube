#pragma once
#include <glm/glm.hpp>
#include <glwrapper/VertexArray.h>

namespace GLW = GLWrapper;

struct VertexPC
{
	glm::vec3 position;
	glm::vec3 color;

	static GLW::VertexStructMember positionMem;
	static GLW::VertexStructMember colorMem;

};
inline GLW::VertexStructMember VertexPC::positionMem{ 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), offsetof(VertexPC, position) };
inline GLW::VertexStructMember VertexPC::colorMem{ 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), offsetof(VertexPC, color) };


