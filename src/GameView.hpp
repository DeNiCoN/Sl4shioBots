/*
  Copyright(c) 2020 DeNiCoN

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
#include <string>
#include <list>
#include "linearmath.h"
#include <vector>
#include <unordered_map>
#include "poolAlloc.h"


namespace Bots
{
#define ALLOCATOR_MAX_GOOM_OBJECTS 64
#define ALLOCATOR_MAX_ARROW_OBJECTS 128

    class BotServer;
    class GameView;

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
        Q = 0,
        W = 1,
        E = 2,
        R = 3,
        A = 4,
        S = 5
    };

    class Entity
    {
        void virtual sync(const char** payload) = 0;
    };

    class Goom : public Entity
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

    class Arrow : public Entity
    {
        friend class BotServer;
        friend class GameView;
    public:
        vec2 getPosition() const { return position; }
        vec2 getVelocity() const { return velocity; }
        std::string getName() const { return name; }
        float getHp() const { return hp; }
        float getMaxHp() const { return maxHp; }
        Arrows getType() const { return type; }
        uint8_t getLevel() const { return level; }
        bool isDashing() const { return dashing; }
        bool isShielding() const { return shielding; }
        bool isAttackReady() const { return attackReady; }
        bool isShieldReady() const { return shieldReady; }
        bool isMain() const { return main; }
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
        bool main;

        void virtual sync(const char** payload);
    };

    class GameView
    {
        friend class BotServer;
    public:
        GameView(BotServer& world) : world(world)
        {
            poolInit(&goomAlloc, new char[ALLOCATOR_MAX_GOOM_OBJECTS * sizeof(Goom)], sizeof(Goom), ALLOCATOR_MAX_GOOM_OBJECTS);
            poolInit(&arrowAlloc, new char[ALLOCATOR_MAX_ARROW_OBJECTS * sizeof(Arrow)], sizeof(Arrow), ALLOCATOR_MAX_ARROW_OBJECTS);
        }
        ~GameView()
        {
            delete[] goomAlloc.buffer;
            delete[] arrowAlloc.buffer;
        }
        const Goom* nearestGoom(vec2 position) const;
        const Arrow* nearestArrow(vec2 position) const;
        const Arrow* getMain() const { return main; }
        vec2 getLeaderPos() const { return leaderPos; }
        void destroyGoom(uint32_t id);
        void destroyArrow(uint32_t id);
    private:
        void sync(const char* payload);
        void reset();
        Arrow* main = nullptr;
        PoolAllocator arrowAlloc;
        PoolAllocator goomAlloc;
        std::unordered_map<uint32_t, Goom*> gooms;
        std::unordered_map<uint32_t, Arrow*> arrows;
        BotServer& world;
        vec2 leaderPos;
        uint32_t id;
    };
}
