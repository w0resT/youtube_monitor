#include "gui/gui.h"
#include "core/core.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!gui::SetupWindow(hInstance))
		return EXIT_FAILURE;

	curl_global_init(CURL_GLOBAL_ALL);

	std::thread(core::Run).detach();

	gui::Run();
	gui::ReleaseWindow(hInstance);

	curl_global_cleanup();

	return EXIT_SUCCESS;
}
