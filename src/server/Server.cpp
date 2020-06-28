#include "server/Server.hpp"

namespace Sl4shioBots
{

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

bool Server::connect()
{
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

    m_endpoint.set_open_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
    {
        //Successfully connected
        //Idk why i'm sending space
        //This is login
        m_endpoint.send(hdl, " ", websocketpp::frame::opcode::TEXT);
    }, &m_endpoint, _1));
    m_endpoint.set_fail_handler(bind([&](client* c, websocketpp::connection_hdl hdl)
    {
        //Failed
    }, &m_endpoint, _1));

    m_endpoint.init_asio();
    m_endpoint.start_perpetual();
    m_endpoint_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>
        (&client::run, &m_endpoint);
    m_connected = true;
    return true;
}

bool Server::run()
{
    connect();
    for (int i = 0; i < m_numCores; i++)
    {
        m_cluster.addInstance();
    }
    //TODO later
    //startListeningForCommands();
    return 0;
}
}
