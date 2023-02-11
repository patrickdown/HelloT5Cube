
#include <iostream>
#include <cstring>
#include "HelloT5Cube.h"



int main(int argc, char* argv[])
{
	bool isSuccessfulLaunch = false;
	{
		std::cout << "Command line options: " << std::endl;
		std::cout << "multiview - Use multiview rendering and submit texture array to T5" << std::endl;
		std::cout << "debug - Enable GL_DEBUG_OUTPUT" << std::endl;

		bool multiview = false;
		bool copyTextures = false;
		bool glDebug = false;
		for(int idx = 1; idx < argc; ++idx) {
			if(strncmp(argv[idx], "multiview", 9) == 0) {
				multiview = true;
				copyTextures = false;
			}
			if(strncmp(argv[idx], "multiview-copy", 14) == 0) {
				multiview = true;
				copyTextures = true;
			}
			if(strncmp(argv[idx], "debug", 5) == 0) {
				glDebug = true;
			}

		}
		if(argc > 1) {
		}

		HelloT5Cube hello(1216, 768, "Hello T5 Cube", multiview, copyTextures, glDebug);

		isSuccessfulLaunch = hello.Initialize();
		if(isSuccessfulLaunch)
			hello.Run();
	}
}

