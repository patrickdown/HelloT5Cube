#pragma once
#include <TiltFiveNative.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace T5Wrapper
{

glm::vec2 toGLM(const T5_Vec2& t5Vec2)
{
	return glm::vec2(t5Vec2.x, t5Vec2.y);
}

glm::vec3 toGLM(const T5_Vec3& t5Vec3)
{
	return glm::vec3(t5Vec3.x, t5Vec3.y, t5Vec3.z);
}

glm::quat toGLM(const T5_Quat& t5Quat)
{
	return glm::quat(t5Quat.w, t5Quat.x, t5Quat.y, t5Quat.z);
}

T5_Vec2 toT5(const glm::vec2& glmVec2)
{
	return {glmVec2.x, glmVec2.y};
}

T5_Vec3 toT5(const glm::vec3& glmVec3)
{
	return {glmVec3.x, glmVec3.y, glmVec3.z};
}

T5_Quat toT5(const glm::quat& glmQuat)
{
	return { glmQuat.w, glmQuat.x, glmQuat.y, glmQuat.z};
}
}
