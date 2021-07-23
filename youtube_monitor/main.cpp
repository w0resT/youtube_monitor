#include "gui/gui.h"
#include "core/core.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// Starting with setup window
	Gui gui;
	if ( !gui.SetupWindow( hInstance ) )
		return EXIT_FAILURE;

	curl_global_init( CURL_GLOBAL_ALL );

	// Net & parsing
	std::thread( core::run ).detach( );

	gui.run( );
	gui.ReleaseWindow( hInstance );

	curl_global_cleanup( );

	return EXIT_SUCCESS;
}
