#include <Application.h>

#include "MyApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	MyApp myApp;

	Application* app = Application::GetInstance();
	app->SetDelegate(&myApp);
	app->Run();

	return 0;
}