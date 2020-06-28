#pragma once
#include <vector>

namespace BotBase
{
template<typename BotType, typename Input>
class BotManager
{
    std::vector<BotType> m_bots;
    double m_ups;
public:
    BotManager(double ups = 30) : m_ups(ups) {}

    void run()
    {

    }

    double getUPS() const { return m_ups; }

    void setUPS(double p_ups)
    {

    }
};

template<typename BotManagerType>
class BotManagerCluster
{
    std::vector<BotManagerType> m_managers;
public:
    void addInstance()
    {

    }
};
}
