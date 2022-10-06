#pragma once
#include <memory>
#include <assert.h>

template <typename T>
class call_deleter
{
	public:
	constexpr void operator()(T* arg) const
	{
		arg->Delete();
		delete arg;
	}
};

template<typename T>
using Owned = std::unique_ptr<T, call_deleter<T>>;

template<typename T, typename... Args>
Owned<T> MakeOwned(Args&&... args)
{
	return Owned<T>(new T(std::forward<Args>(args)...));
}