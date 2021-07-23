#include "gui.h"
#include "../helpers/helpers.h"
#include "../net/net.h"

Gui::Gui( ) {
	this->m_lpz_class = nullptr;
	this->m_hwnd = nullptr;
	this->m_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove 
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
}

void Gui::run( ) {
	// Main loop
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while ( msg.message != WM_QUIT ) {
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			continue;
		}

		if ( globals::exit ) {
			break;
		}

		this->MainWindow( );

		// Big optimization 
		using namespace std::chrono_literals;
		std::this_thread::sleep_for( 50ms );
	}
}

bool Gui::SetupWindow( HINSTANCE m_hInstance ) {
	this->m_lpz_class = "YouTube Monitor";
	if ( !helpers::RegisterWindow( m_hInstance, this->m_lpz_class ) ) {
		return false;
	}

	RECT rect_screen;
	GetWindowRect( GetDesktopWindow( ), &rect_screen );
	int x = rect_screen.right / 2 - 300;
	int y = rect_screen.bottom / 2 - 200;

	this->m_hwnd = CreateWindow( this->m_lpz_class, this->m_lpz_class, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, m_hInstance, NULL );
	if ( !this->m_hwnd ) {
		return false;
	}

	if ( !helpers::CreateDeviceD3D( this->m_hwnd ) ) {
		helpers::CleanupDeviceD3D( );
		UnregisterClass( this->m_lpz_class, m_hInstance );
		return false;
	}

	// Show the window
	ShowWindow( this->m_hwnd, SW_SHOWDEFAULT );
	UpdateWindow( this->m_hwnd );

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init( this->m_hwnd );
	ImGui_ImplDX9_Init( globals::d3d_device );

	LoadStyle( );

	return true;
}

void Gui::ReleaseWindow( HINSTANCE m_hInstance ) {
	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );

	helpers::CleanupDeviceD3D( );
	DestroyWindow( this->m_hwnd );
	UnregisterClass( this->m_lpz_class, m_hInstance );
}

void Gui::MainWindow( ) {
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	// Bar menu
	if ( ImGui::BeginMainMenuBar( ) ) {
		if ( ImGui::BeginMenu( "Options" ) ) {
			if ( ImGui::MenuItem( "Exit" ) ) {
				globals::exit = true;
			}

			ImGui::EndMenu( );
		}

		ImGui::EndMainMenuBar( );
	}

	// Main window
	ImGui::SetNextWindowPos( ImVec2( 0, 19 ) );
	ImGui::SetNextWindowSize( ImVec2( WINDOW_WIDTH - 16, WINDOW_HEIGHT - 58 ) );
	if ( ImGui::Begin( "#MainWindow", NULL, this->m_window_flags ) ) {
		float x = ImGui::GetWindowWidth( ) / 2 - 90.f;
		float y = ImGui::GetWindowHeight( ) / 2 - 50.f;

		ImGui::InputText( "Path", &globals::s_path );
		if ( ImGui::Button( "Update" ) ) {
			// Reset
			globals::addresses.clear( );
			globals::parsed_data.clear( );
			helpers::LoadUrls( );
		}
		ImGui::Separator( );

		if ( ImGui::BeginChild( "ListChild", ImVec2( 0, 0 ), true ) ) {
			if ( !globals::parsed_data.empty( ) ) {
				for ( auto& data : globals::parsed_data ) {
					// If title or viewers neither is empty so stream is over
					// and we don't need to check another one
					if ( data.first.empty( ) ) {
						ImGui::TextColored( ImColor( 255.f, 0.f, 0.f ), "Error, stream is probably over" );
					}
					else {
						ImGui::TextWrapped( "Title: %s", data.first.c_str( ) );
						ImGui::TextWrapped( "Viewers: %s", data.second.c_str( ) );
					}
					ImGui::Separator( );
				}
			}
			ImGui::EndChild( );
		}
	}
	ImGui::End( );

	// Rendering
	ImGui::EndFrame( );
	globals::d3d_device->SetRenderState( D3DRS_ZENABLE, FALSE );
	globals::d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	globals::d3d_device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int )( clear_color.x * 255.0f ), ( int )( clear_color.y * 255.0f ), ( int )( clear_color.z * 255.0f ), ( int )( clear_color.w * 255.0f ) );
	globals::d3d_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

	if ( globals::d3d_device->BeginScene( ) >= 0 ) {
		ImGui::Render( );
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
		globals::d3d_device->EndScene( );
	}

	HRESULT result = globals::d3d_device->Present( NULL, NULL, NULL, NULL );

	// Handle loss of D3D9 device
	if ( result == D3DERR_DEVICELOST && globals::d3d_device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET ) {
		helpers::ResetDevice( );
	}
}

void Gui::LoadStyle( ) {
	ImGuiStyle& style = ImGui::GetStyle( );

	style.WindowRounding = 0.f;
	style.WindowPadding.x = 8.f;
	style.WindowPadding.y = 3.f;
	style.FramePadding.x = 5.f;
	style.FramePadding.y = 3.f;
	style.FrameRounding = 0.f;
	style.ItemSpacing.x = 5.f;
	style.ItemSpacing.y = 4.f;
	style.ItemInnerSpacing.x = 5.f;
	style.ItemInnerSpacing.y = 4.f;
	style.TabRounding = 4.f;

	ImGui::GetStyle( ) = style;
	ImVec4* colors = style.Colors;
	colors[ ImGuiCol_Text ] = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
	colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
	colors[ ImGuiCol_WindowBg ] = ImVec4( 0.94f, 0.94f, 0.94f, 1.00f );
	colors[ ImGuiCol_ChildBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_PopupBg ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.98f );
	colors[ ImGuiCol_Border ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.31f );
	colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
	colors[ ImGuiCol_FrameBg ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.05f );
	colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.18f );
	colors[ ImGuiCol_TitleBg ] = ImVec4( 0.96f, 0.96f, 0.96f, 1.00f );
	colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.82f, 0.82f, 0.82f, 1.00f );
	colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.51f );
	colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
	colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.98f, 0.98f, 0.98f, 0.53f );
	colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.69f, 0.69f, 0.69f, 0.80f );
	colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.49f, 0.49f, 0.49f, 0.80f );
	colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.49f, 0.49f, 0.49f, 1.00f );
	colors[ ImGuiCol_CheckMark ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.64f );
	colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.64f );
	colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.46f, 0.54f, 0.80f, 0.60f );
	colors[ ImGuiCol_Button ] = ImVec4( 0.00f, 0.00f, 0.02f, 0.08f );
	colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.05f );
	colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.00f, 0.00f, 0.02f, 0.08f );
	colors[ ImGuiCol_Header ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.16f );
	colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.19f );
	colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.24f );
	colors[ ImGuiCol_Separator ] = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
	colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.19f );
	colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.24f );
	colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.56f );
	colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.19f );
	colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.38f );
	colors[ ImGuiCol_PlotLines ] = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
	colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
	colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.45f, 0.00f, 1.00f );
	colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.19f );
	colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.20f, 0.20f, 0.20f, 0.35f );
	colors[ ImGuiCol_DragDropTarget ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.19f );
}
