#pragma once
#include "World.h"
#include <string_view>
#include <unordered_map>
#include <memory>
#include "DefaultBehavior.h"

class ClientRequest;
using ClientRequestPtr = std::unique_ptr<ClientRequest>;

class ClientRequest
{
public:
    virtual void handle(BotServer& server) = 0;
    virtual bool Parse(std::string_view) = 0;

    static ClientRequestPtr ParseRequest(std::string_view);

private:
    static std::unordered_map<std::string_view,
                              std::function<ClientRequestPtr()>> s_requestsFac;
};

namespace Requests
{
    class Test : public ClientRequest
    {
    public:
        void handle(BotServer& server) override
        {
            server.sendMessageToClient("Test successful");
        }

        bool Parse(std::string_view) override { return true; }
    };

    class Connect : public ClientRequest
    {
    public:
        void handle(BotServer& server) override
        {
            auto botPtr =
                std::make_shared<Bot>(m_name, std::make_unique<DefaultBehavior>(), server);
            if (server.connect(move(botPtr)))
                server.sendMessageToClient("Successfully connected");
            else
                server.sendMessageToClient("Error connecting bot");
        }
        bool Parse(std::string_view) override;
    private:
        std::string m_name;
    };

    class Disconnect : public ClientRequest
    {
    public:
        void handle(BotServer& server) override
        {
            if (server.disconnect(m_name))
            {
                server.sendMessageToClient("Successfully disconnected");
            }
            else
            {
                server.sendMessageToClient("Bot not found");
            }
        }
        bool Parse(std::string_view) override;
    private:
        std::string m_name;
    };
}
