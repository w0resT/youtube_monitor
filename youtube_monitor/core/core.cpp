#include "../globals.h"
#include "../helpers/helpers.h"
#include "../net/net.h"
#include "core.h"

void core::run( ) {
	// Loading urls from the file
	if ( !helpers::LoadUrls( ) ) {
		return;
	}

	while ( !globals::exit ) 	{
		// Count of addr = count of threads
		for ( auto& address : globals::addresses ) {
			NetParse net_parse( address );
			net_parse.parse( );

			auto parsed_data = net_parse.GetData( );
			globals::parsed_data.push_back( parsed_data );
		}

		std::this_thread::sleep_for( std::chrono::seconds( UPDATE_TIME ) );

		globals::parsed_data.clear( );
	}
}
