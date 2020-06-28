#pragma once

#include <string>
#include <list>
#include "linearmath.h"
#include <vector>
#include <unordered_map>
#include "poolAlloc.h"
#include "server/Sl4shioWorld.hpp"


namespace Sl4shioBots
{
class Sl4shioGameView
{
public:
    Sl4shioGameView(Sl4shioWorld& world) : world(world)
    {
    }
    ~Sl4shioGameView()
    {
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
