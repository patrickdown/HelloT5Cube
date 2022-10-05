
#include "HelloT5Cube.h"

int main(void)
{
	{
		HelloT5Cube hello(1216, 768,"Hello T5 Cube");

		if(hello.Initialize())
			hello.Run();
	}
	{
		HelloT5Cube hello(1216, 768, "Hello T5 Cube");

		if (hello.Initialize())
			hello.Run();
	}
}

