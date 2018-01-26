#include "MeshObject.h"
#include "Panel3D.h"
#include "Panel2D.h" // To be continued...
#include "../../BIS/BIS/Window.h"


void Test_Panel()
{
	Window window(L"Test_Window", 1280, 720);
	Panel3D panel(100, 100, 0, 0, window.GetWindow());
	
}