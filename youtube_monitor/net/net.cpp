#include "../globals.h"
#include "../helpers/helpers.h"
#include "net.h"

typedef size_t(*CURL_WRITEFUNCTION_PTR)(void*, size_t, size_t, void*);
size_t write_to_string(void* ptr, size_t size, size_t count, void* stream)
{
	static_cast<std::string*>(stream)->append(static_cast<char*>(ptr), 0, size * count);
	return size * count;
}

std::pair<std::string, std::string> net::SendReq(std::string req_adr)
{
	static std::string user_agent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36";

	try
	{
		CURL* curl = curl_easy_init();

		std::string content;
		struct curl_slist* header = NULL;

		header = curl_slist_append(header, "Connection: keep-alive");
		header = curl_slist_append(header, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
		header = curl_slist_append(header, "Accept-Language: ru,en;q=0.9");

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
			curl_easy_setopt(curl, CURLOPT_URL, req_adr.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 9);

			curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<CURL_WRITEFUNCTION_PTR>(write_to_string));
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

			auto res = curl_easy_perform(curl);
			long response_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			curl_easy_cleanup(curl);

			if (res != CURLE_OK || response_code != 200)
				return std::make_pair("", "");
		}

		std::string title, viewers;

		if (!content.empty())
		{
			title = helpers::ParseString("\"title\":{\"runs\":[{\"text\":\"", "\"}]}", content);
			title.erase(std::remove_if(title.begin(), title.end(), [](char c) { return c < 0; }), title.end());

			viewers = helpers::ParseString("{\"videoViewCountRenderer\":{\"viewCount\":{\"runs\":[{\"text\":\"", "\"}]}", content);
			viewers.erase(std::remove_if(viewers.begin(), viewers.end(), [](char c) { return (c < 0) ? true : !isdigit(c); }), viewers.end());

			// Sometimes it can't get title, idk why
			if (title == " .")
				title = "Error";
		}

		ZeroMemory(header, sizeof(header));

		return std::make_pair(title, viewers);
	}
	catch (...)
	{
		return std::make_pair("", "");
	}
}
