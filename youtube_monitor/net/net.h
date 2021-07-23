#pragma once

class NetParse {
public:
	NetParse( std::string address );

	void parse( );
	auto GetData( ) {
		if ( this->m_parsed )
			return std::make_pair( this->m_parsed_title, this->m_parsed_viewers );
		else
			return std::make_pair( std::string( ), std::string( ) );
	}

private:
	static size_t curl_writecallbackfn( void* contents, size_t size, size_t count, void* stream ) {
		static_cast< std::string* >( stream )->append( static_cast< char* >( contents ), size * count );
		return size * count;
	}

private:
	// Read buffer, hold the raw data recieved from curl
	std::string m_raw_data;
	long m_response_code;

	// Parsed data buffer
	std::string m_parsed_title, m_parsed_viewers;
	bool m_parsed;
};