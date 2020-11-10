/*
  Copyright(c) 2019 DeNiCoN

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

#include "Server.hpp"
#include "Bot.hpp"
#include "Messages.hpp"

namespace Bots
{
    void Bot::setAngle(float angle)
    {
        world.endpoint.send(connection_hdl, Messages::setAngle(angle), websocketpp::frame::opcode::BINARY);
    }

    void Bot::dash(float angle)
    {
        world.endpoint.send(connection_hdl, Messages::dash(angle), websocketpp::frame::opcode::BINARY);
    }

    void Bot::shield()
    {
        world.endpoint.send(connection_hdl, Messages::shield(), websocketpp::frame::opcode::BINARY);
    }

    void Bot::leave()
    {
        world.endpoint.send(connection_hdl, Messages::leave(), websocketpp::frame::opcode::BINARY);
    }
    void Bot::upgrade(Upgrades upgrade)
    {
        world.endpoint.send(connection_hdl, Messages::upgrade(upgrade), websocketpp::frame::opcode::BINARY);
    }
}
