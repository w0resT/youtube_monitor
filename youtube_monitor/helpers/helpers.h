#pragma once
#include "../globals.h"

namespace helpers {
	extern ATOM RegisterWindow( HINSTANCE hInstance, LPCTSTR lpzClassName );
	extern bool CreateDeviceD3D( HWND hWnd );
	extern void CleanupDeviceD3D( );
	extern void ResetDevice( );
	extern std::string ParseString( std::string before, std::string after, std::string source );
	extern bool LoadUrls( );
}