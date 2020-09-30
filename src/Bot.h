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
#pragma once
#include <memory>
#include <string>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <ctype.h>
#include "GameView.h"

class Bot;
class BotServer;
class VBehavior;

using Bot_ptr = std::shared_ptr<Bot>;

struct BotStats
{
	unsigned int kills;
	unsigned int deaths;
	unsigned int shieldUsed;
	unsigned int maxDashCombo;
	unsigned int dashes;
	unsigned int maxLevel;
	unsigned int damageGot;
	unsigned int damageDealt;
};

struct BotState
{
	vec2 position;
	unsigned int exp;
	unsigned int lvl;
	unsigned int dashCooldown;
	unsigned int dashCombo;
	float vision;
	uint8_t upgradesAvailable;
	Arrows arrow;

};

class VBehavior
{
	friend class Bot;
public:
	virtual ~VBehavior() {}
	void virtual onPlayingStart() = 0;
	void virtual onUpgradeAvailable(uint8_t count) {}
	void virtual update(std::chrono::duration<double> delta) = 0;
protected:
	Bot* bot;
};

using VBehaviorPtr = std::unique_ptr<VBehavior>;
class Bot
{
	friend class BotServer;
public:
	Bot(std::string name, VBehaviorPtr behavior, BotServer& world)
			: name(name), behavior(move(behavior)), world(world), view(world)
			{ this->behavior->bot = this; }
	const std::string& getName() const { return name; }
	websocketpp::connection_hdl getConnectionHandle() const { return connection_hdl; }
	void setAngle(float angle);
	void dash(float angle);
	void shield();
	void upgrade(Upgrades upgrade);

	const GameView& getGameView() const { return view; }
	const BotState& getBotState() const { return state; }
	uint32_t getId() const { return id; }
private:
	void leave();
	BotStats stats;
	BotState state;
	websocketpp::connection_hdl connection_hdl;
	std::string name;
	uint32_t id;
	VBehaviorPtr behavior;
	BotServer& world;
	GameView view;
};
