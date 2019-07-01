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
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define ASIO_STANDALONE

#include <websocketpp/common/random.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/thread.hpp>
#include "World.h"
#include <vector>
#include <sstream>
#include <chrono>
#include "DefaultBehavior.h"

using clk = std::chrono::high_resolution_clock;

World world("ws://45.32.233.97:9000");
std::stringstream messages;
websocketpp::lib::mutex messages_mutex;

bool run = true;

void consoleReader()
{
	std::string message;
	while (run)
	{
		std::getline(std::cin, message);
		messages_mutex.lock();
		messages << message;
		messages_mutex.unlock();
	}
}

void handleMessage(std::string message)
{
	std::string mes;
	std::stringstream stream(message);
	stream >> mes;
	if (mes == "quit")
	{
		run = false;
	}
	else if (mes == "connect")
	{
		int n;
		stream >> n;
		for (int i = 0; i < n; i++)
		{
			std::string name;
			stream >> name;
			Bot_ptr bot = Bot_ptr(new Bot(name , *(new DefaultBehavior()), world));
			world.connect(bot);
		}
	}
	else if (mes.substr(0, 5) == "leave")
	{

	}
}

int main() 
{
	world.init();
	std::cout << "Enter connect [count] [names] to connect bots" << '\n';
	auto console_thread = websocketpp::lib::thread(consoleReader);
	std::string message;

	clk::time_point current = clk::now();
	clk::time_point last = clk::now();
	auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(current - last);
	while (run)
	{
		current = clk::now();
		delta = current - last;
		if (messages.rdbuf()->in_avail() != 0 && messages_mutex.try_lock())
		{
			std::getline(messages, message);
			messages_mutex.unlock();
			handleMessage(message);
		}
		
		world.update(delta);

		last = current;
	}

	console_thread.join();
	return 0;
}