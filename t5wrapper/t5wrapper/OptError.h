#pragma once
#include <TiltFiveNative.h>
#include <variant>



namespace T5Wrapper
{

template<typename TYPE>
struct OptError : public std::variant<T5_Result, TYPE>
{
	bool TryGet(TYPE& value)
	{
		if (auto* pValue = std::get_if<TYPE>(this))
		{
			value = *pValue;
			return true;
		}
		return false;
	}

	bool IsValid()
	{
		return std::holds_alternative<TYPE>(*this);
	}

	TYPE GetValue()
	{
		if (auto* pError = std::get_if<TYPE>(this))
		{
			return *pError;
		}
		return TYPE();
	}

	T5_Result GetError()
	{
		if (auto* pError = std::get_if<T5_Result>(this))
		{
			return *pError;
		}
		return T5_SUCCESS;
	}

	std::string GetErrorMessage()
	{
		if (auto* pError = std::get_if<T5_Result>(this))
		{
			return t5GetResultMessage(*pError);
		}
		return std::string();
	}
};

}