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

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

enum Arrows
{

};

enum Upgrades
{

};

namespace Messages
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


	size_t start(Arrows arrow, std::string name, std::iostream& stream);
	size_t leave(std::iostream& stream);
	size_t setAngle(float angle, std::iostream& stream);
	size_t dash(float angle, std::iostream& stream);
	size_t shield(std::iostream& stream);
	size_t upgrade(Upgrades upgrade, std::iostream& stream);

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
	std::unordered_map<websocketpp::connection_hdl, Bot_ptr> hdls;
	std::unordered_map <Messages::ICodes, std::function<void(websocketpp::connection_hdl, const char*)>> mHandlers;
};