#include "../../Test/Testing.h"
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	Testing::testOne();
    return 0;
}


