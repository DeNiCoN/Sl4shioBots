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

#include "DefaultBehavior.hpp"
#include "Utils.hpp"

void DefaultBehavior::onPlayingStart()
{
    bot->setAngle(0);
}

void DefaultBehavior::update(std::chrono::duration<double> delta)
{
    delay += delta;
    if (delay > updateDelay)
    {
        delay = std::chrono::duration<double>(0);
        const Arrow* arrow = bot->getGameView().nearestArrow(bot->getGameView().getMain()->getPosition());
        const Goom* goom = bot->getGameView().nearestGoom(bot->getGameView().getMain()->getPosition());
        float angle;
        vec2 arrowRelativePos;
        vec2 goomRelativePos;
        vec2 nearestPos;
        if (arrow && goom)
        {
            arrowRelativePos = vec2Sub(bot->getGameView().getMain()->getPosition(), arrow->getPosition());
            goomRelativePos = vec2Sub(bot->getGameView().getMain()->getPosition(), goom->getPosition());
            if (vec2LengthSquared(goomRelativePos) < vec2LengthSquared(arrowRelativePos))
            {
                angle = utils::clockwiseAngle(goomRelativePos);
                nearestPos = goomRelativePos;
            }
            else
            {
                angle = utils::clockwiseAngle(arrowRelativePos);
                nearestPos = arrowRelativePos;
            }
        }
        else if (goom)
        {
            goomRelativePos = vec2Sub(bot->getGameView().getMain()->getPosition(), goom->getPosition());
            angle = utils::clockwiseAngle(goomRelativePos);
            nearestPos = goomRelativePos;
        }
        else if (arrow)
        {
            arrowRelativePos = vec2Sub(bot->getGameView().getMain()->getPosition(), arrow->getPosition());
            angle = utils::clockwiseAngle(arrowRelativePos);
            nearestPos = arrowRelativePos;
        }
        else
        {
            vec2 leaderRelativePos = vec2Sub(bot->getGameView().getMain()->getPosition(), bot->getGameView().getLeaderPos());

            if (vec2Length(leaderRelativePos) < 10)
                nearestPos = vec2Sub(bot->getGameView().getMain()->getPosition(), {200.0, 200.0});
            else
                nearestPos = leaderRelativePos;

            angle = utils::clockwiseAngle(nearestPos);
        }
        bot->setAngle(angle);
        if (vec2LengthSquared(nearestPos) < 168)
        {
			bot->dash(angle);
		}
	}
}
