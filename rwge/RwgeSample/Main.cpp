#include <RwgeApplication.h>

#include "MaterialTestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	MaterialTestApp myApp;
	RApplication::SetDelegate(&myApp);

	RApplication application;
	application.Run();

	return 0;
}