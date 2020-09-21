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

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <chrono>
#include "linearmath.h"
#include "Bot.h"
#include <list>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Goom;
class Arrow;

class BotServer
{
	friend class Bot;
public:
	BotServer(std::string uri);
	~BotServer();
	bool run(std::chrono::duration<double>);
	bool connect(Bot_ptr bot);
	void close() { m_shouldClose = true; }

private:
	void init();
	void update(std::chrono::duration<double> delta);

	std::string uri;
	std::vector<Bot_ptr> activeBots;
	bool m_shouldClose = false;

	websocketpp::lib::mutex messageQueueMutex;
	std::queue<std::pair<Bot_ptr, client::message_ptr>> messagesQueue;
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> endpoint_thread;
	client endpoint;

	void handleMessage(std::pair<Bot_ptr, client::message_ptr> entry);

	void onSetup(Bot_ptr bot, const char* payload);
	void onStartPlaying(Bot_ptr bot, const char* payload);
	void onGameOver(Bot_ptr bot, const char* payload);
	void onSpecArrow(Bot_ptr bot, const char* payload);
	void onSync(Bot_ptr bot, const char* payload);
	void onDamage(Bot_ptr bot, const char* payload);
	void onDestroyArrow(Bot_ptr bot, const char* payload);
	void onDashArrow(Bot_ptr bot, const char* payload);
	void onDestroyGoom(Bot_ptr bot, const char* payload);
	void onHitGoom(Bot_ptr bot, const char* payload);
	void onHitArrow(Bot_ptr bot, const char* payload);
	void onRecoilArrow(Bot_ptr bot, const char* payload);
	void onEatGoo(Bot_ptr bot, const char* payload);
	void onUpgradesAvailable(Bot_ptr bot, const char* payload);
	void onSetVision(Bot_ptr bot, const char* payload);
	void onDash(Bot_ptr bot, const char* payload);
	void onDashCombo(Bot_ptr bot, const char* payload);
	void onShield(Bot_ptr bot, const char* payload);
	void onShieldUsed(Bot_ptr bot, const char* payload);
	void onKill(Bot_ptr bot, const char* payload);
	void onSetLeaderboard(Bot_ptr bot, const char* payload);
};
