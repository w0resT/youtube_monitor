#include "gui/gui.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!gui::SetupWindow(hInstance))
		return EXIT_FAILURE;

	gui::Run();

	gui::ReleaseWindow(hInstance);

	return EXIT_SUCCESS;
}
