#pragma once
#include <string>
#include <list>
#include "linearmath.h"
#include <vector>
#include <unordered_map>
#include "poolAlloc.h"

namespace Sl4shioBots
{

enum Arrows
{
BLUE = 0,
SCOUT = 1,
FROZEN = 2,
FLAME = 3,
CRUISER = 4,
ASSASIN = 5
};

enum Upgrades
{
HP = 0,
DAMAGE = 1,
SPEED = 2,
VISION = 3,
COOLDOWN = 4,
SHIELD_DAMAGE = 5
};

class Entity
{
    void virtual sync(const char** payload) = 0;
};

class Goom final : public Entity
{
    friend class GameView;
public:
    vec2 getPosition() const { return position; }
    vec2 getVelocity() const { return velocity; }
    float getHP() const { return hp; }
private:
    vec2 position;
    vec2 velocity;
    float hp;
    float size;
    void virtual sync(const char** payload);
};

class Arrow final : public Entity
{
    friend class BotServer;
    friend class GameView;
public:
    vec2 getPosition() const { return position; }
    vec2 getVelocity() const { return velocity; }
    const std::string& getName() const { return name; }
    float getHp() const { return hp; }
    float getMaxHp() const { return maxHp; }
    Arrows getType() const { return type; }
    uint8_t getLevel() const { return level; }
    bool isDashing() const { return dashing; }
    bool isShielding() const { return shielding; }
    bool isAttackReady() const { return attackReady; }
    bool isShieldReady() const { return shieldReady; }
    bool isBot() const { return bot; }
private:
    vec2 position;
    vec2 velocity;
    std::string name;
    float cooldown;
    float shieldCooldown;
    float hp;
    float maxHp;
    uint32_t id;
    Arrows type;
    uint8_t level;
    bool dashing;
    bool shielding;
    bool attackReady;
    bool shieldReady;
    bool bot;

    void virtual sync(const char** payload);
};

class Sl4shioWorld
{
    std::unordered_map<uint32_t, Goom> gooms;
    std::unordered_map<uint32_t, Goom> arrows;
};
}
