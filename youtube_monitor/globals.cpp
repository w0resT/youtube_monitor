#include "globals.h"

namespace globals
{
	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3d_device = NULL;
	D3DPRESENT_PARAMETERS d3d_params = {};

	std::vector< std::pair< std::string, std::string > > parsed_data;
	std::vector < std::string > addresses;

	std::string s_path = "G:\\youtube_urls.txt";

	bool exit = false;
}