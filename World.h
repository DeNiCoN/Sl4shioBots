/*
MIT License

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
#include "Bot.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <chrono>
#include "linearmath.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

enum Arrows
{
	STANDART = 0
};

enum Upgrades
{
	
};

namespace Messages
{
	template<typename T>
	inline T read(const char** payload)
	{
		T data = *reinterpret_cast<const T*>(*payload);
		*payload += sizeof(T);
		return data;
	}

	namespace Input
	{
		enum ICodes
		{
			SETUP = 0, //0
			START_PLAYING = 1, //1
			GAME_OVER = 2, //2
			SPEC_ARROW = 3,//3
			SYNC = 4,//4
			DAMAGE = 5,//5
			DESTROY_ARROW = 6, //6
			DASH_ARROW = 7,//7
			DESTROY_GOOM = 8,//8
			HIT_GOOM = 9,//9
			HIT_ARROW = 10,//10
			RECOIL_ARROW = 11,//11
			EAT_GOO = 12,//12
			UPGRADES_AVAILABLE = 13,//13
			SET_VISION = 14,//14
			DASH = 15,//15
			DASH_COMBO = 16,//16
			SHIELD = 17,//17
			SHIELD_USED = 18,//18
			KILL = 19,//19
			SET_LEADERBOARD = 20//20
		};
	}
	namespace Output
	{
		enum OCodes
		{
			SET_OBS_POS = 0,
			START = 1,
			SPECTATE = 2,
			LEAVE = 3,
			SET_MOVEMENT = 4,
			DASH = 5,
			SHIELD = 6,
			UPGRADE = 7
		};
	}

	std::string setObsPos(vec2 coords);
	std::string start(Arrows arrow, std::string name);
	std::string leave();
	std::string setAngle(float angle);
	std::string dash(float angle);
	std::string shield();
	std::string upgrade(Upgrades upgrade);

}

class World
{
public:
	World(std::string uri);
	~World();
	void init();
	void update(std::chrono::duration<double> delta);
	bool connect(Bot_ptr bot);
private:
	bool initialized = false;
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> endpoint_thread;
	client endpoint;
	std::string uri;
	std::vector<Bot_ptr> activeBots;

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