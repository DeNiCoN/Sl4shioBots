#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define ASIO_STANDALONE

#include "World.h"
#include <vector>
#include <sstream>
#include <chrono>
#include <websocketpp/common/thread.hpp>

using clock = std::chrono::high_resolution_clock;

World world("ws://45.32.233.97:9000");
std::stringstream messages;
websocketpp::lib::mutex messages_mutex;

bool run = true;

void consoleReader()
{
	std::string message;
	while (run)
	{
		std::cin >> message;
		messages_mutex.lock();
		messages << message;
		messages_mutex.unlock;
	}
}

void handleMessage(std::string message)
{
	if (message == "quit")
	{
		run = false;
	}
	else if (message.substr(0, 7) == "connect")
	{
		Bot_ptr bot = Bot_ptr(new Bot("test"));
		world.connect(bot);
	}
	else if (message.substr(0, 5) == "leave")
	{

	}
}

int main() 
{
	world.init();

	auto console_thread = websocketpp::lib::thread(consoleReader);
	std::string message;

	clock::time_point current = clock::now();
	clock::time_point last = clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(current - last);
	while (run)
	{
		current = clock::now();
		delta = current - last;
		if (messages.rdbuf()->in_avail() != 0 && messages_mutex.try_lock())
		{
			messages >> message;
			messages_mutex.unlock();
			handleMessage(message);
		}
		
		world.update(delta);

		last = current;
	}

	console_thread.join();
}