#include <Application.h>
#include <AppDelegate.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	AppDelegate appDelegate;

	Application app(&appDelegate);
	app.Run();

	return 0;
}