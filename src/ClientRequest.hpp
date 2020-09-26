#pragma once
#include "World.h"
#include <string_view>

class ClientRequest
{
    virtual void handle(BotServer& server) = 0;
    virtual bool Parse(std::string_view) = 0;
};
