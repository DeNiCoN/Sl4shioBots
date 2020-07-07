/*
Copyright(c) 2019 DeNiCoN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <thread>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include "World.h"
#include "DefaultBehavior.h"

std::stringstream g_stream;

size_t mywrite(char* c, size_t size, size_t nmemb, void* userp)
{
	g_stream.write(static_cast<char*>(c), size*nmemb);
	return size*nmemb;
}

//sends http request through libcurl
//and returns response through stream
//very likely to be used only one time
std::istream& get(const char* url)
{
	g_stream.clear();
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mywrite);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return g_stream;
}

//returns sl4sh io server ip and port
//parses json using jsonpp
std::string getServerIpPort(const char* url)
{
	nlohmann::json json;
	get(url) >> json;
	return json.front().front()["address"].get<std::string>();
}


int main()
{
	std::string uri = getServerIpPort("http://sl4sh.io/servers.json");
	BotServer server {"ws://" + uri};
	server.init();

	for(int i = 0; i < 50; i++) {
	Bot_ptr bot = Bot_ptr(new Bot("" , *(new DefaultBehavior()), server));
	server.connect(bot);
	}

	using clk = std::chrono::high_resolution_clock;
	clk::time_point current = clk::now();
	clk::time_point last = clk::now();
	auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(current - last);
	while (true)
	{
		delta = current - last;
		std::this_thread::sleep_for(std::chrono::milliseconds(17) - delta);
		current = clk::now();
		server.update(std::chrono::milliseconds(17));

		last = current;
	}
}
