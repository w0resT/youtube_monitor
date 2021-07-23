#pragma once
#include "../globals.h"

namespace gui {
	extern bool SetupWindow( HINSTANCE m_hInstance );
	extern void ReleaseWindow( HINSTANCE m_hInstance );
	extern void run( );

	// widnows
	extern void MainWindow( );

	extern void LoadStyle( );

	extern LPCTSTR lpz_class;
	extern HWND hwnd;

	// bools for windows


	extern ImGuiWindowFlags window_flags;
}