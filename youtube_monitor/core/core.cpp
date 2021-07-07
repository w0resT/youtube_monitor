#include "../globals.h"
#include "../helpers/helpers.h"
#include "../net/net.h"
#include "core.h"

void core::Run()
{
	// Loading urls from the file
	if (!helpers::LoadUrls())
		return;

	while (!globals::exit)
	{
		// Count of addr = count of threads
		for (auto req_adr : globals::v_adrs)
		{
			auto [title, viewers] = net::SendReq(req_adr);
			globals::v_titles.push_back(std::make_pair(title, viewers));
		}

		std::this_thread::sleep_for(std::chrono::seconds(UPDATE_TIME));

		globals::v_titles.clear();
	}
}
