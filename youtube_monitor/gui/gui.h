#pragma once
#include "../globals.h"

class Gui {
public:
	Gui( );

	bool SetupWindow( HINSTANCE m_hInstance );
	void ReleaseWindow( HINSTANCE m_hInstance );
	void run( );

private:
	void MainWindow( );
	void LoadStyle( );

private:
	ImGuiWindowFlags m_window_flags;
	LPCTSTR m_lpz_class;
	HWND m_hwnd;
};