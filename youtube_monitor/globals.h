#pragma once
#include <d3d9.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

// curl
#include "curl/curl.h"
#pragma comment(lib, "libcurl.lib")

// d3d9
#pragma comment(lib, "d3d9.lib")

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 300
#define DIRECTINPUT_VERSION 0x0800
#define UPDATE_TIME 10

namespace globals
{
	extern LPDIRECT3D9 d3d;
	extern LPDIRECT3DDEVICE9 d3d_device;
	extern D3DPRESENT_PARAMETERS d3d_params;

	extern std::vector< std::pair< std::string, std::string > > v_titles;
	extern std::vector < std::string > v_adrs;

	extern std::string s_path;

	extern bool exit;
}