#include <Application.h>

#include "MaterialTestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	MaterialTestApp myApp;

	Application application;
	application.SetDelegate(&myApp);
	application.Run();

	return 0;
}