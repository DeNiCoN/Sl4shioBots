#pragma once
#include "World.h"
#include <string_view>
#include <unordered_map>
#include <memory>

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

class TestRequest : public ClientRequest
{
public:
    void handle(BotServer& server) override
    {
        server.sendMessageToClient("Test successful");
    }

    bool Parse(std::string_view) override { return true; }
};
