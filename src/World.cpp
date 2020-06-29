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
#include "World.h"


BotServer::BotServer(std::string uri) : uri(uri)
{

}

BotServer::~BotServer()
{
	if (initialized)
	{
		endpoint.stop_perpetual();
		endpoint_thread->join();
		activeBots.clear();
	}
}

void BotServer::init()
{
	initialized = true;

	endpoint.clear_access_channels(websocketpp::log::alevel::all);
	endpoint.clear_error_channels(websocketpp::log::elevel::all);

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
	endpoint_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>
		(&client::run, &endpoint);
}

void BotServer::update(std::chrono::duration<double> delta)
{
	while (!messagesQueue.empty())
	{
		messageQueueMutex.lock();
		auto m = messagesQueue.front();
		messagesQueue.pop_front();
		messageQueueMutex.unlock();
		handleMessage(m);
	}

	for (auto bot : activeBots)
	{
		/*
		if(bot->view.main)
			if (bot->view.main->cooldown > 0)
			{
				bot->view.main->cooldown -= delta.count();
			}
			else
			{
				bot->view.main->attackReady = true;
			}
		*/
		if (bot->view.main)
			bot->behavior.update(delta);
	}
}

bool BotServer::connect(Bot_ptr bot)
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
		messageQueueMutex.lock();
		messagesQueue.push_back(std::make_pair(bot, m));
		messageQueueMutex.unlock();
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
	return { (uint8_t)Messages::Output::LEAVE };
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
	std::string payload({ (uint8_t)Messages::Output::UPGRADE });
	payload += (uint8_t)upgrade;
	return payload;
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
	std::cout << "setup";
	bot->id = Messages::read<uint32_t>(&payload);
	bot->view.id = bot->id;
	endpoint.send(bot->getConnectionHandle(), Messages::setObsPos(Messages::read<vec2>(&payload)), websocketpp::frame::opcode::BINARY);
	endpoint.send(bot->getConnectionHandle(), Messages::start((Arrows)(rand() % 6), bot->getName()), websocketpp::frame::opcode::BINARY);
}
void BotServer::onStartPlaying(Bot_ptr bot, const char* payload)
{
	std::cout << "start playing";
	bot->state.upgradesAvailable = Messages::read<uint8_t>(&payload);
	bot->state.vision = Messages::read<float>(&payload);
	activeBots.push_back(bot);
	bot->behavior.onPlayingStart();
}

void BotServer::onGameOver(Bot_ptr bot, const char* payload)
{
	std::cout << "game over";

	activeBots.erase(std::find(activeBots.begin(), activeBots.end(), bot));
	bot->view.reset();
	bot->stats.deaths++;
	endpoint.send(bot->getConnectionHandle(), Messages::start((Arrows)(rand() % 6), bot->getName()), websocketpp::frame::opcode::BINARY);
}

void BotServer::onSpecArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "spec arrow";
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
	std::cout << "damage";
}

void BotServer::onDestroyArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "destroy arrow";
	uint32_t id = Messages::read<uint32_t>(&payload);
	bot->view.destroyArrow(id);
}

void BotServer::onDashArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "dash arrow";
}

void BotServer::onDestroyGoom(Bot_ptr bot, const char* payload)
{
	std::cout << "destroy goom";
	uint32_t id = Messages::read<uint32_t>(&payload);
	bot->view.destroyGoom(id);
}

void BotServer::onHitGoom(Bot_ptr bot, const char* payload)
{
	std::cout << "hit goom";
}

void BotServer::onHitArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "hit arrow";
}

void BotServer::onRecoilArrow(Bot_ptr bot, const char* payload)
{
	std::cout << "recoil arrow";
}

void BotServer::onEatGoo(Bot_ptr bot, const char* payload)
{
	std::cout << "eat goo";
	
}

void BotServer::onUpgradesAvailable(Bot_ptr bot, const char* payload)
{
	std::cout << "upgrades available";
	bot->behavior.onUpgradeAvailable(Messages::read<uint8_t>(&payload));
}

void BotServer::onSetVision(Bot_ptr bot, const char* payload)
{
	std::cout << "set vision";
	bot->state.vision = Messages::read<float>(&payload);
}

void BotServer::onDash(Bot_ptr bot, const char* payload)
{
	std::cout << "dash";
	//bot->view.main->cooldown = Messages::read<uint32_t>(&payload);
	//bot->view.main->attackReady = false;
	bot->stats.dashes++;
}

void BotServer::onDashCombo(Bot_ptr bot, const char* payload)
{
	std::cout << "dash combo";
	//bot->view.main->cooldown = 0;
	//bot->view.main->attackReady = true;
	if (bot->state.dashCombo = Messages::read<uint32_t>(&payload) > bot->stats.maxDashCombo)
	{
		bot->stats.maxDashCombo = bot->state.dashCombo;
	}

}

void BotServer::onShield(Bot_ptr bot, const char* payload)
{
	std::cout << "shield";
}

void BotServer::onShieldUsed(Bot_ptr bot, const char* payload)
{
	bot->stats.shieldUsed++;
	std::cout << "shield used";
}

void BotServer::onKill(Bot_ptr bot, const char* payload)
{
	std::cout << "kill";
	bot->stats.kills++;
}

void BotServer::onSetLeaderboard(Bot_ptr bot, const char* payload)
{
	std::cout << "set leaderboard";
}
