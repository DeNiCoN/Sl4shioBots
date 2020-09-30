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

#include "GameView.h"
#include <array>
#include "World.h"
#include "Messages.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "ClientRequest.hpp"
#include <thread>

BotServer::BotServer(std::string uri, uint16_t server_port)
	: uri(uri), server_port(server_port)
{
	endpoint.clear_access_channels(websocketpp::log::alevel::all);
	endpoint.clear_error_channels(websocketpp::log::elevel::all);

	endpoint.set_open_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
	{
		endpoint.send(hdl, " ", websocketpp::frame::opcode::TEXT);
	}, &endpoint, ::_1));
	endpoint.set_fail_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
	{
	}, &endpoint, ::_1));

	endpoint.init_asio();
	endpoint.start_perpetual();
	endpoint_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>
		(&client::run, &endpoint);
}

BotServer::~BotServer()
{
	endpoint.stop_perpetual();
	endpoint_thread->join();
	activeBots.clear();
	for (const auto& [name, bot] : connectedBots)
	{
		bot->leave();
	}
}

void BotServer::sendMessageToClient(std::string_view str)
{
	current_socket.send(asio::buffer(str.data(), str.length()));
}

bool BotServer::run(std::chrono::duration<double> secondsPerUpdate)
{
	std::thread listen_thread([&]()
	{
		using namespace asio::ip;
		while (!m_shouldClose)
		{
			try
			{
				tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), server_port));
				acceptor.accept(current_socket);
				std::cout << "Client connected" << std::endl;
				while(true)
				{
					std::array<char, 256> message;
					asio::error_code error;
					auto length = current_socket.read_some(asio::buffer(message), error);
					if (error == asio::error::eof)
					{
						std::cout << "Client disconnected" << std::endl;
						break; // Connection closed cleanly by peer.
					}
					else if (error)
						throw asio::system_error(error);

					auto request = ClientRequest::ParseRequest(
						std::string_view(message.data(), length));

					if (request)
					{
						requestsQueue.push(move(request));
					}
					else
					{
						sendMessageToClient("Error");
					}
				}
			}
			catch (std::exception& e)
			{
				current_socket.close();
				std::cerr << e.what() << std::endl;
			}
		}
	});

	using clk = std::chrono::high_resolution_clock;
	clk::time_point current = clk::now();
	clk::time_point last = clk::now();
	auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(current - last);
	while (!m_shouldClose)
	{
		delta = current - last;
		std::this_thread::sleep_for(secondsPerUpdate - delta);
		current = clk::now();
		update(secondsPerUpdate);

		last = current;
	}
	listen_thread.join();
	return 0;
}

void BotServer::update(std::chrono::duration<double> delta)
{
	{
		std::lock_guard lock(messageQueueMutex);
		while (!messagesQueue.empty())
		{
			auto& m = messagesQueue.front();
			handleMessage(m);
			messagesQueue.pop();
		}
		while(!requestsQueue.empty())
		{
			auto& request = requestsQueue.front();
			request->handle(*this);
			requestsQueue.pop();
		}
	}

	for (auto bot : activeBots)
	{
		if (bot->view.main)
			bot->behavior->update(delta);
	}
}

bool BotServer::connect(Bot_ptr bot)
{
	websocketpp::lib::error_code ec;
	client::connection_ptr connection = endpoint.get_connection("ws://" + uri, ec);
	if (ec)
	{
		std::cerr << ec.message() << std::endl;
		return false;
	}
	bot->connection_hdl = connection->get_handle();
	connection->set_message_handler([&, botptr = bot](websocketpp::connection_hdl hdl, client::message_ptr m)
	{
		std::lock_guard lock(messageQueueMutex);
		messagesQueue.push(std::make_pair(botptr, m));
	});

	endpoint.connect(connection);
	connectedBots[bot->getName()] = bot;
	return true;
}

bool BotServer::disconnect(const std::string& name)
{
	auto it = std::find_if(activeBots.begin(), activeBots.end(), [&name](const Bot_ptr ptr)
	{
		return ptr->getName() == name;
	});

	if (it != activeBots.end())
	{
		activeBots.erase(it);
	}
	if (connectedBots.count(name))
	{
		connectedBots.at(name)->leave();
		connectedBots.erase(name);
		return true;
	}
	return false;
}

void BotServer::handleMessage(std::pair<Bot_ptr, client::message_ptr> entry)
{
	Bot_ptr bot = entry.first;
	const char* payload = entry.second->get_payload().c_str();
	switch ((Messages::Input::ICodes) * (payload++))
	{
	case 0: onSetup(bot, payload); break;
	case 1: onStartPlaying(bot, payload); break;
	case 2: onGameOver(bot, payload); break;
	case 3: onSpecArrow(bot, payload); break;
	case 4: onSync(bot, payload); break;
	case 5: onDamage(bot, payload); break;
	case 6: onDestroyArrow(bot, payload); break;
	case 7: onDashArrow(bot, payload); break;
	case 8: onDestroyGoom(bot, payload); break;
	case 9: onHitGoom(bot, payload); break;
	case 10: onHitArrow(bot, payload); break;
	case 11: onRecoilArrow(bot, payload); break;
	case 12: onEatGoo(bot, payload); break;
	case 13: onUpgradesAvailable(bot, payload); break;
	case 14: onSetVision(bot, payload); break;
	case 15: onDash(bot, payload); break;
	case 16: onDashCombo(bot, payload); break;
	case 17: onShield(bot, payload); break;
	case 18: onShieldUsed(bot, payload); break;
	case 19: onKill(bot, payload); break;
	case 20: onSetLeaderboard(bot, payload); break;
	default:
		std::cout << "unresolved code\n";
		break;
	}
}

void BotServer::onSetup(Bot_ptr bot, const char* payload)
{
	bot->id = Messages::read<uint32_t>(&payload);
	bot->view.id = bot->id;
	endpoint.send(bot->getConnectionHandle(), Messages::setObsPos(Messages::read<vec2>(&payload)), websocketpp::frame::opcode::BINARY);
	endpoint.send(bot->getConnectionHandle(), Messages::start((Arrows)(rand() % 6), bot->getName()), websocketpp::frame::opcode::BINARY);
}
void BotServer::onStartPlaying(Bot_ptr bot, const char* payload)
{
	bot->state.upgradesAvailable = Messages::read<uint8_t>(&payload);
	bot->state.vision = Messages::read<float>(&payload);
	activeBots.push_back(bot);
	bot->behavior->onPlayingStart();
}

void BotServer::onGameOver(Bot_ptr bot, const char* payload)
{

	activeBots.erase(std::find(activeBots.begin(), activeBots.end(), bot));
	bot->view.reset();
	bot->stats.deaths++;
	endpoint.send(bot->getConnectionHandle(),
				  Messages::start((Arrows)(rand() % 6), bot->getName()),
				  websocketpp::frame::opcode::BINARY);
}

void BotServer::onSpecArrow(Bot_ptr bot, const char* payload)
{
}

void BotServer::onSync(Bot_ptr bot, const char* payload)
{
	float levelRatio = Messages::read<float>(&payload);
	uint32_t leaderId = Messages::read<uint32_t>(&payload);
	vec2 pos = Messages::read<vec2>(&payload);
	bot->view.leaderPos = pos;
	bot->view.sync(payload);
}

void BotServer::onDamage(Bot_ptr bot, const char* payload)
{
}

void BotServer::onDestroyArrow(Bot_ptr bot, const char* payload)
{
	uint32_t id = Messages::read<uint32_t>(&payload);
	bot->view.destroyArrow(id);
}

void BotServer::onDashArrow(Bot_ptr bot, const char* payload)
{
}

void BotServer::onDestroyGoom(Bot_ptr bot, const char* payload)
{
	uint32_t id = Messages::read<uint32_t>(&payload);
	bot->view.destroyGoom(id);
}

void BotServer::onHitGoom(Bot_ptr bot, const char* payload)
{
}

void BotServer::onHitArrow(Bot_ptr bot, const char* payload)
{
}

void BotServer::onRecoilArrow(Bot_ptr bot, const char* payload)
{
}

void BotServer::onEatGoo(Bot_ptr bot, const char* payload)
{

}

void BotServer::onUpgradesAvailable(Bot_ptr bot, const char* payload)
{
	bot->behavior->onUpgradeAvailable(Messages::read<uint8_t>(&payload));
}

void BotServer::onSetVision(Bot_ptr bot, const char* payload)
{
	bot->state.vision = Messages::read<float>(&payload);
}

void BotServer::onDash(Bot_ptr bot, const char* payload)
{
	//bot->view.main->cooldown = Messages::read<uint32_t>(&payload);
	//bot->view.main->attackReady = false;
	bot->stats.dashes++;
}

void BotServer::onDashCombo(Bot_ptr bot, const char* payload)
{
	//bot->view.main->cooldown = 0;
	//bot->view.main->attackReady = true;
	if ((bot->state.dashCombo = Messages::read<uint32_t>(&payload)) > bot->stats.maxDashCombo)
	{
		bot->stats.maxDashCombo = bot->state.dashCombo;
	}

}

void BotServer::onShield(Bot_ptr bot, const char* payload)
{
}

void BotServer::onShieldUsed(Bot_ptr bot, const char* payload)
{
	bot->stats.shieldUsed++;
}

void BotServer::onKill(Bot_ptr bot, const char* payload)
{
	bot->stats.kills++;
}

void BotServer::onSetLeaderboard(Bot_ptr bot, const char* payload)
{
}

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
//parses json
std::string getServerIpPort(const char* url)
{
	nlohmann::json json;
	get(url) >> json;
	return json.front().front()["address"].get<std::string>();
}

int main(int argc, char** argv)
{
	uint16_t port = 34534;

	std::string uri = getServerIpPort("http://sl4sh.io/servers.json");

	BotServer server(uri, port);

	srand(time(NULL));
	server.run(std::chrono::duration<double>(1.0/60.0));

    return 0;
}
