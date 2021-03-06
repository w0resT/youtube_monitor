#include "helpers.h"
#include "../resource.h"
#include "../wndproc.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ATOM helpers::RegisterWindow( HINSTANCE hInstance, LPCTSTR lpzClassName ) {
	WNDCLASS window_class = { 0 };
	window_class.lpfnWndProc = reinterpret_cast< WNDPROC >( WndProc );
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = lpzClassName;
	window_class.hCursor = LoadCursor( NULL, IDC_ARROW );
	window_class.hbrBackground = reinterpret_cast< HBRUSH >( COLOR_APPWORKSPACE );
	window_class.hIcon = reinterpret_cast< HICON >( LoadImage( hInstance, MAKEINTRESOURCE( IDI_ICON1 ), IMAGE_ICON, 32, 32, 0 ) );

	return RegisterClass( &window_class );
}

bool helpers::CreateDeviceD3D( HWND hWnd ) {
	if ( ( globals::d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ) {
		return false;
	}

	// Create the D3DDevice
	ZeroMemory( &globals::d3d_params, sizeof( globals::d3d_params ) );
	globals::d3d_params.Windowed = TRUE;
	globals::d3d_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	globals::d3d_params.BackBufferFormat = D3DFMT_UNKNOWN;
	globals::d3d_params.EnableAutoDepthStencil = TRUE;
	globals::d3d_params.AutoDepthStencilFormat = D3DFMT_D16;
	globals::d3d_params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if ( globals::d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &globals::d3d_params, &globals::d3d_device ) < 0 ) {
		return false;
	}

	return true;
}

void helpers::CleanupDeviceD3D( ) {
	if ( globals::d3d_device ) { 
		globals::d3d_device->Release( ); 
		globals::d3d_device = NULL; 
	}

	if ( globals::d3d ) { 
		globals::d3d->Release( ); 
		globals::d3d = NULL; 
	}
}

void helpers::ResetDevice( ) {
	ImGui_ImplDX9_InvalidateDeviceObjects( );

	HRESULT m_hResult = globals::d3d_device->Reset( &globals::d3d_params );
	if ( m_hResult == D3DERR_INVALIDCALL ) {
		IM_ASSERT( 0 );
	}

	ImGui_ImplDX9_CreateDeviceObjects( );
}

std::string helpers::ParseString( std::string before, std::string after, std::string source ) {
	if ( !before.empty( ) && !after.empty( ) && !source.empty( ) && ( source.find( before ) != std::string::npos ) && ( source.find( after ) != std::string::npos ) ) 	{
		std::string t = strstr( source.c_str( ), before.c_str( ) );
		t.erase( 0, before.length( ) );
		std::string::size_type loc = t.find( after, 0 );
		t = t.substr( 0, loc );
		return t;
	}
	else {
		return "";
	}
}

bool helpers::LoadUrls( ) {
	std::ifstream in( globals::s_path, std::ios::in );
	if ( in.is_open( ) ) {
		std::string line;
		while ( getline( in, line ) ) {
			// If we got only stream id
			if ( line.find( ".com" ) == std::string::npos ) {
				line.insert( 0, std::string( "https://www.youtube.com/" ) );
			}

			globals::addresses.push_back( line );
		}
		in.close( );

		return true;
	}
	return false;
}
