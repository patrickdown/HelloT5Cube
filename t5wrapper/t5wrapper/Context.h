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

class Context 
{
	protected:
	T5_Context handle = 0;

	public:
	explicit operator T5_Context() { return handle; }

	T5_Result Create(std::string applicationId, std::string version);
	void Delete();

	OptGlassesList GetConnectedGlasses();

	private:
	int maxRetryCount = 4;
	unsigned long long retryWaitTime = 100;

	T5_Result WaitForService(std::function<T5_Result()> func);
};

}
