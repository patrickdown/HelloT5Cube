
#include "HelloT5Cube.h"

int main(void)
{
	bool isSuccessfulFirstLaunch = false;
	{
		HelloT5Cube hello(1216, 768,"Hello T5 Cube");

		isSuccessfulFirstLaunch = hello.Initialize();
		if(isSuccessfulFirstLaunch)
			hello.Run();
	}
}

