#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include "server/BotManager.hpp"
#include <vector>
#include <thread>
#include <string>

namespace Sl4shioBots
{

//using Sl4shioBot = BotBase::Bot<SlGameView, SlOutput>;

class Server
{
    bool m_connected = false;
    int m_numCores = std::thread::hardware_concurrency();
    websocketpp::client<websocketpp::config::asio_client> m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_endpoint_thread;

    BotBase::BotManager<Sl4shioBot, Sl4shioUpdate> m_botManager;
    //BotBase::BotManagerCluster<BotBase::BotManager<Sl4shioBot, SlInput>> m_cluster;
    //Connect through websocket to game server
    bool connect();
public:
    const std::string ipport;

    //Connect to game server and start listening for client commands
    bool run();

    Server(std::string p_ipport)
        : ipport(p_ipport)
    {
        //Run on one thread if can't get number of threads
        m_numCores = m_numCores ? m_numCores : 1;
    }
    ~Server();
};

}
