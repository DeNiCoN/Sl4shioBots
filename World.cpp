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

#include "World.h"

World::World(std::string uri) : uri(uri)
{

}

World::~World()
{
	if (initialized)
	{
		endpoint.stop_perpetual();
		endpoint_thread->join();
		activeBots.clear();
	}
}

void World::init()
{
	initialized = true;

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
	bot->connection_hdl = connection->get_handle();
	connection->set_message_handler([&, bot](websocketpp::connection_hdl hdl, client::message_ptr m)
	{
		const char* payload = m->get_payload().c_str();
		switch ((Messages::Input::ICodes)*(payload++))
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
	});

	endpoint.connect(connection);
	return true;
}

std::string Messages::setObsPos(vec2 coords)
{
	std::string payload({ (uint8_t)Messages::Output::SET_OBS_POS });
	payload.append((const char*)&coords.x, sizeof(float));
	payload.append((const char*)&coords.y, sizeof(float));
	return payload;
}

std::string Messages::start(Arrows arrow, std::string name)
{
	std::string payload({ (uint8_t)Messages::Output::START });
	payload += (uint8_t)arrow;
	payload += name;
	return payload;
}

std::string Messages::leave()
{
	return { (uint8_t)Messages::Output::START };
}

std::string Messages::setAngle(float angle)
{
	std::string payload({ (uint8_t)Messages::Output::SET_MOVEMENT });
	payload.append((const char*)& angle, sizeof(float));
	return payload;
}

std::string Messages::dash(float angle)
{
	std::string payload({ (uint8_t)Messages::Output::DASH });
	payload.append((const char*)& angle, sizeof(float));
	return payload;
}

std::string Messages::shield()
{
	return { (uint8_t)Messages::Output::SHIELD };
}

std::string Messages::upgrade(Upgrades upgrade)
{
	std::string payload({ (uint8_t)Messages::Output::START });
	payload += (uint8_t)upgrade;
	return payload;
}

void World::onSetup(Bot_ptr bot, const char* payload)
{
	std::cout << "setup";
	bot->id = Messages::read<uint32_t>(&payload);
	endpoint.send(bot->getConnectionHandle(), Messages::setObsPos(Messages::read<vec2>(&payload)), websocketpp::frame::opcode::BINARY);
	endpoint.send(bot->getConnectionHandle(), Messages::start(Arrows::STANDART, bot->getName()), websocketpp::frame::opcode::BINARY);
}
void World::onStartPlaying(Bot_ptr bot, const char* payload)
{
	std::cout << "start playing";
	bot->state.upgradesAvailable = Messages::read<uint8_t>(&payload);
	bot->state.vision = Messages::read<float>(&payload);
	bot->behavior.onPlayingStart(bot);
}

void World::onGameOver(Bot_ptr bot, const char* payload)
{
	std::cout << "game over";
	bot->stats.deaths++;
}

void World::onSpecArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "spec arrow";
}

void World::onSync(Bot_ptr bot, const char* payload)
{
	float levelRatio = Messages::read<float>(&payload);
	uint32_t leaderId = Messages::read<uint32_t>(&payload);
	vec2 pos = Messages::read<vec2>(&payload);
	std::cout << "sync";
}

void World::onDamage(Bot_ptr bot, const char* payload)
{
	std::cout << "damage";
}

void World::onDestroyArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "destroy arrow";
}

void World::onDashArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "dash arrow";
}

void World::onDestroyGoom(Bot_ptr bot, const char* payload)
{
	std::cout << "destroy goom";
}

void World::onHitGoom(Bot_ptr bot, const char* payload)
{
	std::cout << "hit goom";
}

void World::onHitArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "hit arrow";
}

void World::onRecoilArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "recoil arrow";
}

void World::onEatGoo(Bot_ptr bot, const char* payload)
{
	std::cout << "eat goo";
	
}

void World::onUpgradesAvailable(Bot_ptr bot, const char* payload)
{
	std::cout << "upgrades available";
	bot->state.upgradesAvailable = Messages::read<uint8_t>(&payload);
}

void World::onSetVision(Bot_ptr bot, const char* payload)
{
	std::cout << "set vision";
	bot->state.vision = Messages::read<float>(&payload);
}

void World::onDash(Bot_ptr bot, const char* payload)
{
	std::cout << "dash";
	bot->state.dashCooldown = Messages::read<uint32_t>(&payload);
	bot->stats.dashes++;
}

void World::onDashCombo(Bot_ptr bot, const char* payload)
{
	std::cout << "dash combo";
	if (bot->state.dashCombo = Messages::read<uint32_t>(&payload) > bot->stats.maxDashCombo)
	{
		bot->stats.maxDashCombo = bot->state.dashCombo;
	}

}

void World::onShield(Bot_ptr bot, const char* payload)
{
	std::cout << "shield";
}

void World::onShieldUsed(Bot_ptr bot, const char* payload)
{
	bot->stats.shieldUsed++;
	std::cout << "shield used";
}

void World::onKill(Bot_ptr bot, const char* payload)
{
	std::cout << "kill";
	bot->stats.kills++;
}

void World::onSetLeaderboard(Bot_ptr bot, const char* payload)
{
	std::cout << "set leaderboard";
}
