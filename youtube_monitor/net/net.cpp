#include "../globals.h"
#include "../helpers/helpers.h"
#include "net.h"

NetParse::NetParse( std::string address ) {
	try {
		this->m_response_code = 0l;
		this->m_parsed = false;

		CURL* curl = curl_easy_init( );
		if ( curl ) {
			curl_easy_setopt( curl, CURLOPT_URL, address.c_str( ) );
			curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, this->curl_writecallbackfn );
			curl_easy_setopt( curl, CURLOPT_WRITEDATA, &this->m_raw_data );
			curl_easy_perform( curl );
			curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &this->m_response_code );
			curl_easy_cleanup( curl );
		}
	}
	catch ( ... ) {
		// Error handling
	}
}

void NetParse::parse( ) {
	// Got some problems with net request
	if ( this->m_response_code != 200 ) {
		return;
	}

	// Content buffer is empty
	if ( this->m_raw_data.empty( ) ) {
		return;
	}

	// Parsing title
	this->m_parsed_title = helpers::ParseString( "\"title\":{\"runs\":[{\"text\":\"", "\"}]}", this->m_raw_data );
	this->m_parsed_title.erase( std::remove_if( this->m_parsed_title.begin( ), this->m_parsed_title.end( ), 
								[ ]( char c ) { return c < 0; } ), this->m_parsed_title.end( ) );

	// Parsing viewers
	this->m_parsed_viewers = helpers::ParseString( "{\"videoViewCountRenderer\":{\"viewCount\":{\"runs\":[{\"text\":\"", "\"}]}", this->m_raw_data );
	this->m_parsed_viewers.erase( std::remove_if( this->m_parsed_viewers.begin( ), this->m_parsed_viewers.end( ), 
								  [ ]( char c ) { return ( c < 0 ) ? true : !isdigit( c ); } ), this->m_parsed_viewers.end( ) );
	
	// Sometimes its can't get title, idk why
	if ( this->m_parsed_title == " ." )
		this->m_parsed_title = "Error";

	if ( !this->m_parsed_title.empty( ) && !this->m_parsed_viewers.empty( ) ) {
		this->m_parsed = true;
	}
}
