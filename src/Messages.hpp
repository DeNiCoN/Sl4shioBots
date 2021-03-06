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
#include "linearmath.h"
#include <string>

namespace Bots
{
    namespace Messages
    {

        namespace Input
        {
            enum ICodes
            {
                SETUP = 0,
                START_PLAYING = 1,
                GAME_OVER = 2,
                SPEC_ARROW = 3,
                SYNC = 4,
                DAMAGE = 5,
                DESTROY_ARROW = 6,
                DASH_ARROW = 7,
                DESTROY_GOOM = 8,
                HIT_GOOM = 9,
                HIT_ARROW = 10,
                RECOIL_ARROW = 11,
                EAT_GOO = 12,
                UPGRADES_AVAILABLE = 13,
                SET_VISION = 14,
                DASH = 15,
                DASH_COMBO = 16,
                SHIELD = 17,
                SHIELD_USED = 18,
                KILL = 19,
                SET_LEADERBOARD = 20
            };
        }

        namespace Output
        {
            enum OCodes
            {
                SET_OBS_POS = 0,
                START = 1,
                SPECTATE = 2,
                LEAVE = 3,
                SET_MOVEMENT = 4,
                DASH = 5,
                SHIELD = 6,
                UPGRADE = 7
            };
        }


        template<typename T>
        inline T read(const char** payload)
        {
            T data = *reinterpret_cast<const T*>(*payload);
            *payload += sizeof(T);
            return data;
        }

        inline std::string setObsPos(vec2 coords)
        {
            std::string payload({ (uint8_t)Messages::Output::SET_OBS_POS });
            payload.reserve(sizeof(uint8_t) + sizeof(float) * 2);
            payload.append((const char*)&coords.x, sizeof(float));
            payload.append((const char*)&coords.y, sizeof(float));
            return payload;
        }

        inline std::string start(Arrows arrow, std::string name)
        {
            std::string payload({ (uint8_t)Messages::Output::START });
            payload += (uint8_t)arrow;
            payload += name;
            return payload;
        }

        inline std::string leave()
        {
            return { (uint8_t)Messages::Output::LEAVE };
        }

        inline std::string setAngle(float angle)
        {
            std::string payload({ (uint8_t)Messages::Output::SET_MOVEMENT });
            payload.reserve(sizeof(uint8_t) + sizeof(float));
            payload.append((const char*)& angle, sizeof(float));
            return payload;
        }

        inline std::string dash(float angle)
        {
            std::string payload({ (uint8_t)Messages::Output::DASH });
            payload.reserve(sizeof(uint8_t) + sizeof(float));
            payload.append((const char*)& angle, sizeof(float));
            return payload;
        }

        inline std::string shield()
        {
            return { (uint8_t)Messages::Output::SHIELD };
        }

        inline std::string upgrade(Upgrades upgrade)
        {
            std::string payload({ (uint8_t)Messages::Output::UPGRADE });
            payload.reserve(sizeof(uint8_t) * 2);
            payload += (uint8_t)upgrade;
            return payload;
        }

    }
}
