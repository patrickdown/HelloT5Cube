#pragma once
#include <glad/gl.h>
#include <util/Wrapper.h>

namespace GLWrapper
{


template<typename INHERITED>
class GLWrapperBase : public WrapperBase<GLuint, INHERITED> {};

template<typename INHERITED>
class GLBindingWrapper : public WrapperBase<GLuint, INHERITED> 
{
	public:
	template <typename... Args>
	void Bind(Args&&... args)
	{
		INHERITED::BindHandle(WrapperBase<GLuint, INHERITED>::Handle(), std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Unbind(Args&&... args)
	{
		INHERITED::BindHandle(0, std::forward<Args>(args)...);
	}

};

}