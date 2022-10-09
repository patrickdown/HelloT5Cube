#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


namespace GLApplication 
{
	

	class Transform
	{
		glm::vec3 position;
		glm::quat orientation;
		glm::vec3 scale;

		public:
		Transform() 
		: position(0,0,0), orientation(1,0,0,0), scale(1,1,1) {}

		Transform& SetPosition(glm::vec3 const& position)
		{
			this->position = position;
			return *this;
		}		
		
		Transform& SetPosition(float x, float y, float z)
		{
			this->position = glm::vec3(x,y,z);
			return *this;
		}


		glm::vec3 GetPosition()
		{
			return position;
		}

		Transform& SetOrientation(glm::quat const& orientation)
		{
			this->orientation = orientation;
			return *this;
		}

		glm::quat GetOrientation()
		{
			return orientation;
		}

		Transform& SetScale(glm::vec3 const& scale)
		{
			this->scale = scale;
			return *this;
		}

		Transform& SetScale(float scale)
		{
			this->scale = glm::vec3(scale, scale, scale);
			return *this;
		}

		glm::vec3 GetScale()
		{
			return scale;
		}

		Transform& SetAngleAxis(float angle, glm::vec3 const& axis)
		{
			orientation = glm::angleAxis(angle, axis);
			return *this;
		}

		Transform& SetEuler(float angleX, glm::vec3 eulerAngles)
		{
			orientation = glm::quat(eulerAngles);
			return *this;
		}

		Transform& SetEuler(float angleX, float angleY, float angleZ)
		{
			orientation =  glm::quat(glm::vec3(angleX, angleY, angleZ));
			return *this;
		}

		glm::vec3 TransformPointToLocalFrame(glm::vec3 const& point)
		{
			auto localPt = point - position;
			localPt = localPt / scale;
			localPt = glm::rotate(orientation, localPt);
			return localPt;
		}

		glm::vec3 TransformPointToParentFrame(glm::vec3 const& point)
		{
			auto parentPt = glm::rotate(glm::inverse(orientation), point);
			parentPt = parentPt * scale;
			parentPt = position + parentPt;
			return parentPt;
		}

		glm::mat4 MatrixToLocalFrame()
		{
			auto localMat = glm::translate(-position);
			localMat = glm::scale(1.0f/scale) * localMat;
			localMat = glm::mat4_cast(orientation) * localMat;
			return localMat;
		}

		glm::mat4 MatrixToParentFrame()
		{
			auto parentMat = glm::mat4_cast(glm::inverse(orientation));
			parentMat = glm::scale( scale) * parentMat;
			parentMat = glm::translate(position) * parentMat;
			return parentMat;
		}
	};

}

