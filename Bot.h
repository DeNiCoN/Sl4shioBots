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
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define ASIO_STANDALONE
#include <memory>
#include <string>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <ctype.h>



enum Arrows
{
	STANDART = 0
};

enum Upgrades
{

};

class Bot;
class World;
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
	unsigned int exp;
	unsigned int lvl;
	unsigned int dashCooldown;
	unsigned int dashCombo;
	float vision;
	uint8_t upgradesAvailable;
	Arrows arrow;

};

class Bot
{
	friend class World;
public:
	Bot(std::string name, VBehavior& behavior, World& world) : name(name), behavior(behavior), world(world) {}
	std::string getName() const { return name; }
	websocketpp::connection_hdl getConnectionHandle() const { return connection_hdl; }
	void setAngle(float angle);
	void dash(float angle);
	void shield();
	void upgrade(Upgrades upgrade);
private:
	BotStats stats;
	BotState state;
	websocketpp::connection_hdl connection_hdl;
	std::string name;
	uint32_t id;
	VBehavior& behavior;
	World& world;
};


class VBehavior
{
public:
	void virtual onPlayingStart(Bot_ptr bot) = 0;
};