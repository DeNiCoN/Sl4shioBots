#include "World.h"

World::World(std::string uri) : uri(uri)
{
	mHandlers[Messages::ICodes::SETUP] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "setup";
		endpoint.send(hdl, " ", websocketpp::frame::opcode::TEXT);
	};
	mHandlers[Messages::ICodes::START_PLAYING] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "start playing";
	};
	mHandlers[Messages::ICodes::GAME_OVER] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "game over";
	};
	mHandlers[Messages::ICodes::SPEC_ARROW] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "spec arrow";
	};
	mHandlers[Messages::ICodes::SYNC] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "sync";
	};
	mHandlers[Messages::ICodes::DAMAGE] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "damage";
	};
	mHandlers[Messages::ICodes::DESTROY_ARROW] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "destroy arrow";
	};
	mHandlers[Messages::ICodes::DASH_ARROW] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "dash arrow";
	};
	mHandlers[Messages::ICodes::DESTROY_GOOM] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "destroy goom";
	};
	mHandlers[Messages::ICodes::HIT_GOOM] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "hit goom";
	};
	mHandlers[Messages::ICodes::HIT_ARROW] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "hit arrow";
	};
	mHandlers[Messages::ICodes::RECOIL_ARROW] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "recoil arrow";
	};
	mHandlers[Messages::ICodes::EAT_GOO] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "eat goo";
	};
	mHandlers[Messages::ICodes::UPGRADES_AVAILABLE] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "upgrades available";
	};
	mHandlers[Messages::ICodes::SET_VISION] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "set vision";
	};
	mHandlers[Messages::ICodes::DASH] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "dash";
	};
	mHandlers[Messages::ICodes::DASH_COMBO] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "dash combo";
	};
	mHandlers[Messages::ICodes::SHIELD] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "shield";
	};
	mHandlers[Messages::ICodes::SHIELD_USED] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "shield used";
	};
	mHandlers[Messages::ICodes::KILL] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "kill";
	};
	mHandlers[Messages::ICodes::SET_LEADERBOARD] = [&](websocketpp::connection_hdl hdl, const char* payload)
	{
		std::cout << "set leaderboard";
	};

}

World::~World()
{
	if (initialized)
	{
		endpoint.stop_perpetual();
		endpoint_thread->join();
	}
}

void World::init()
{
	initialized = true;
	endpoint.set_message_handler([&](websocketpp::connection_hdl hdl, client::message_ptr m)
		{
			const char* payload = m->get_payload().c_str();
			mHandlers[(Messages::ICodes)(*payload)](hdl, payload + 1);
		});
	endpoint.set_open_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
		{
			std::cout << "Successfully connected";
			endpoint.send(hdl, " ", websocketpp::frame::opcode::TEXT);
			

		}, &endpoint, ::_1));
	endpoint.set_fail_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
		{
			std::cout << "Failed to connect";


		}, &endpoint, ::_1));
	endpoint.init_asio();
	endpoint.start_perpetual();
	endpoint_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &endpoint);
}

void World::update(std::chrono::duration<double> delta)
{
	
}

bool World::connect(Bot_ptr bot)
{
	websocketpp::lib::error_code ec;
	client::connection_ptr connection = endpoint.get_connection(uri, ec);
	if (ec)
	{
		std::cout << ec.message();
		return false;
	}
	endpoint.connect(connection);
	hdls[connection->get_handle()] = bot;
	return true;
}

size_t Messages::start(Arrows arrow, std::string name, std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::START << (uint8_t)arrow << name;
	return 3 + name.size();
}

size_t Messages::leave(std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::LEAVE;
	return sizeof(uint8_t);
}

size_t Messages::setAngle(float angle, std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::SET_MOVEMENT << angle;
	return sizeof(uint8_t) + sizeof(float);
}

size_t Messages::dash(float angle, std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::DASH << angle;
	return sizeof(uint8_t) + sizeof(float);
}

size_t Messages::shield(std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::SHIELD;
	return sizeof(uint8_t);
}

size_t Messages::upgrade(Upgrades upgrade, std::iostream& stream)
{
	stream << (uint8_t)Messages::OCodes::UPGRADE << (uint8_t)upgrade;
	return sizeof(uint8_t) + sizeof(uint8_t);
}
