#pragma once
#include <util/Wrapper.h>
#include <TiltFiveNative.h>
#include <variant>
#include <chrono>
#include <iostream>
#include <functional>
#include "OptError.h"

namespace T5Wrapper
{

using OptGlassesList = OptError<std::vector<std::string>>;

class Context : public WrapperWithErrorCode<T5_Context, T5_Result, Context>
{
	public:
	static bool CreateHandle(T5_Context& handle, T5_Result& errorCode, std::string applicationId, std::string version);

	static void DeleteHandle(T5_Context& handle);

	OptGlassesList GetConnectedGlasses();

	private:
	int maxRetryCount = 4;
	unsigned long long retryWaitTime = 100;

	T5_Result WaitForService(std::function<T5_Result()> func);
};

}
