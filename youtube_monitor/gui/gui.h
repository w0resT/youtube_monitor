#pragma once
#include "../globals.h"

namespace gui
{
	extern bool SetupWindow(HINSTANCE m_hInstance);
	extern void ReleaseWindow(HINSTANCE m_hInstance);
	extern void Run();

	// widnows
	extern void MainWindow();


	extern void LoadStyle();

	extern LPCTSTR lpz_class;
	extern HWND hwnd;

	// bools for windows
	//extern bool show_game_window;
	//extern bool show_enter_window;

	extern ImGuiWindowFlags window_flags;
}