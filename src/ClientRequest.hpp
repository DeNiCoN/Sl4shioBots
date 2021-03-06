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
#include "Server.hpp"
#include <string_view>
#include <unordered_map>
#include <memory>
#include "DefaultBehavior.hpp"

namespace Bots
{
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

        class Stats : public ClientRequest
        {
        public:
            void handle(BotServer& server) override
            {
                if (server.getConnectedBots().count(m_name))
                {
                    const auto& botStats = server.getConnectedBots().at(m_name)->getBotStats();
                    server.sendMessageToClient("\nkills: " + std::to_string(botStats.kills)
                                               + "\ndeaths: " + std::to_string(botStats.deaths)
                                               + "\nshieldUsed: " + std::to_string(botStats.shieldUsed)
                                               + "\nmaxDashCombo: " + std::to_string(botStats.maxDashCombo)
                                               + "\ndashes: " + std::to_string(botStats.dashes)
                                               + "\nmaxLevel: " + std::to_string(botStats.maxLevel)
                                               + "\ndamageGot: " + std::to_string(botStats.damageGot)
                                               + "\ndamageDealt: " + std::to_string(botStats.damageDealt));
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

        class State : public ClientRequest
        {
        public:
            void handle(BotServer& server) override
            {
                if (std::count_if(server.getActiveBots().begin(), server.getActiveBots().end(),
                                  [this](const auto& bot) { return m_name == bot->getName(); }))
                {
                    const auto& botStats = server.getConnectedBots().at(m_name)->getBotState();
                    const auto& botView = server.getConnectedBots().at(m_name)->getGameView().getMain();
                    server.sendMessageToClient("\nposition: " + std::to_string(botView->position.x)
                                               + "," + std::to_string(botView->position.y)
                                               + "\nlvl: " + std::to_string(botView->level)
                                               + "\ndashCooldown: "
                                               + std::to_string(botView->cooldown)
                                               + "\nmaxHp: "
                                               + std::to_string(botView->maxHp)
                                               + "\nhp: " + std::to_string(botView->hp)
                                               + "\nupgradesAvailable: "
                                               + std::to_string(botStats.upgradesAvailable)
                                               + "\narrow: "
                                               + std::to_string((int)botView->type));
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
}
