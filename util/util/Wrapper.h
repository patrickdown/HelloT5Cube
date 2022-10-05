#pragma once
#include <memory>
#include <functional>
#include <assert.h>


template<typename WRAPPED_TYPE, typename INHERITED>
class WrapperBase
{
	protected:
	struct WrapImpl
	{
		WRAPPED_TYPE value;
		WrapImpl()  {}
		~WrapImpl() { INHERITED::DeleteHandle(value); }
	};
	using WrapType = std::shared_ptr<WrapImpl>;
	WrapType wrapped;
	public:
	WrapperBase() = default;

	template <typename... Args>
	static INHERITED Make(Args&&... args)
	{
		INHERITED newThing;
		newThing.Initialize(std::forward<Args>(args)...);
		return newThing;
	}

	template <typename... Args>
	bool Initialize(Args&&... args)
	{
		wrapped = std::make_shared<WrapImpl>();
		if (!INHERITED::CreateHandle(wrapped->value, std::forward<Args>(args)...))
		{
			wrapped.reset();
			return false;
		}
		return true;
	}

	bool IsEmpty() { return (bool)wrapped; }

	const WRAPPED_TYPE& Handle() 
	{
		assert(wrapped);
		return wrapped->value; 
	}

};


template<typename WRAPPED_TYPE, typename ERROR_TYPE, typename INHERITED>
class WrapperWithErrorCode
{
	protected:
	struct WrapImpl
	{
		WRAPPED_TYPE value;
		WrapImpl() {}
		~WrapImpl() { INHERITED::DeleteHandle(value); }
	};
	using WrapType = std::shared_ptr<WrapImpl>;
	WrapType wrapped;
	ERROR_TYPE errorCode;
	public:
	WrapperWithErrorCode() = default;

	template <typename... Args>
	static INHERITED Make(Args&&... args)
	{
		INHERITED newThing;
		newThing.Initialize(std::forward<Args>(args)...);
		return newThing;
	}

	template <typename... Args>
	bool Initialize(Args&&... args)
	{
		wrapped = std::make_shared<WrapImpl>();

		if (!INHERITED::CreateHandle(wrapped->value, errorCode, std::forward<Args>(args)...))
		{
			wrapped.reset();
			return false;
		}
		return true;
	}

	bool IsEmpty() { return (bool)wrapped; }

	const WRAPPED_TYPE& Handle()
	{
		assert(wrapped);
		return wrapped->value;
	}

	ERROR_TYPE ErrorCode()
	{
		return errorCode;
	}

};
