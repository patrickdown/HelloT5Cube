#include <t5wrapper/Context.h>

#define IDENTIFIER_BUFFER_SIZE 1024
#define GLASSES_BUFFER_SIZE    1024
#define PARAM_BUFFER_SIZE      1024
#define WAND_BUFFER_SIZE       4


namespace T5Wrapper {

T5_Result Context::Create(std::string applicationId, std::string version)
{
	assert(handle == 0);
	T5_ClientInfo clientInfo;
	clientInfo.applicationId = applicationId.c_str();
	clientInfo.applicationVersion = version.c_str();

	return t5CreateContext(&handle, &clientInfo, nullptr);
}

void Context::Delete()
{
	if(handle != 0) {
		t5DestroyContext(&handle);
		handle = 0;
	}
}



OptGlassesList Context::GetConnectedGlasses()
{
	assert(handle != 0);

	size_t bufferSize = GLASSES_BUFFER_SIZE;
	char glassesListBuffer[GLASSES_BUFFER_SIZE];
	T5_Result err = WaitForService([this, &glassesListBuffer, &bufferSize] ()
	{ 
		return t5ListGlasses(handle, glassesListBuffer, &bufferSize); 
	});

	if (!err)
	{
		std::vector<std::string> glassesList;
		size_t glassesCount = 0;

		const char* buffPtr = glassesListBuffer;
		for (;;)
		{
			// Get the length of the string and exit if we've read the
			// terminal string (Zero length)
			size_t remainingBuffLen = GLASSES_BUFFER_SIZE - (buffPtr - glassesListBuffer);
			size_t stringLength = strnlen(buffPtr, remainingBuffLen);
			if (stringLength == 0)
			{
				break;
			}
			else if (stringLength == remainingBuffLen)
			{
				std::cerr << "Glasses list overflow" << std::endl;
			}

			glassesList.push_back(buffPtr);
			glassesCount++;

			// Advance through the returned values
			buffPtr += stringLength;
			if (buffPtr > (glassesListBuffer + GLASSES_BUFFER_SIZE))
			{
				break;
			}
		}
		return OptGlassesList(glassesList);
	}
	return OptGlassesList(err);
}

T5_Result Context::WaitForService(std::function<T5_Result()> func)
{
	T5_Result err;
	bool waitingForService = false;
	for (int i = 0; i < maxRetryCount; ++i)
	{
		err = func();
		if (!err)
		{
			return err;
		}
		else if (err == T5_ERROR_NO_SERVICE)
		{
			if (!waitingForService)
			{
				std::cout << "Waiting for service..." << std::endl;
				waitingForService = true;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(retryWaitTime));
	}
	return err;
}

}

