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
#include "GameView.hpp"
#include "Server.hpp"
#include <algorithm>
#include "Messages.hpp"

namespace Bots
{
    const Goom* GameView::nearestGoom(vec2 position) const
    {
        if (gooms.empty())
            return nullptr;
        auto goom = std::min_element(gooms.begin(), gooms.end(), [position](std::pair<unsigned int, Goom*> left, std::pair<unsigned int, Goom*> right) -> bool
        {
            return vec2LengthSquared(vec2Sub(left.second->getPosition(), position)) < vec2LengthSquared(vec2Sub(right.second->getPosition(), position));
        });
        return (*goom).second;
    }

    const Arrow* GameView::nearestArrow(vec2 position) const
    {
        if (arrows.empty())
            return nullptr;
        auto arrow = std::min_element(arrows.begin(), arrows.end(), [position](std::pair<unsigned int, Arrow*> left, std::pair<unsigned int, Arrow*> right) -> bool
        {
            return vec2LengthSquared(vec2Sub(left.second->getPosition(), position)) < vec2LengthSquared(vec2Sub(right.second->getPosition(), position));
        });
        return (*arrow).second;
    }

    void GameView::sync(const char* payload)
    {
        //goo's
        unsigned int n = Messages::read<uint32_t>(&payload);
        for (unsigned int i = 0; i < n; i++)
        {	//new goo
            uint32_t id = Messages::read<uint32_t>(&payload);
            vec2 position = Messages::read<vec2>(&payload);

        }
        n = Messages::read<uint32_t>(&payload);
        for (unsigned int i = 0; i < n; i++)
        {	//goo went out of screen
            uint32_t id = Messages::read<uint32_t>(&payload);


        }

        //gooms
        n = Messages::read<uint32_t>(&payload);
        for (unsigned int i = 0; i < n; i++)
        {	//goom went out of screen
            uint32_t id = Messages::read<uint32_t>(&payload);
            destroyGoom(id);
        }

        n = Messages::read<uint32_t>(&payload);
        for (unsigned int i = 0; i < n; i++)
        {	//new goom
            uint32_t id = Messages::read<uint32_t>(&payload);
            auto g = gooms.find(id);
            if (g == gooms.end())
            {
                Goom* goom = new (poolAlloc(&goomAlloc)) Goom();
                goom->size = Messages::read<float>(&payload);
                goom->sync(&payload);
                gooms[id] = goom;
            }
            else
            {
                g->second->sync(&payload);
            }

        }

        n = Messages::read<uint32_t>(&payload);
        for (unsigned int i = 0; i < n; i++)
        {	//arrow went out of screen
            uint32_t id = Messages::read<uint32_t>(&payload);
            if (id != this->id)
                destroyArrow(id);
        }

        n = Messages::read<uint32_t>(&payload);
        //arrows
        for (unsigned int i = 0; i < n; i++)
        {
            uint32_t id = Messages::read<uint32_t>(&payload);

            if (id != this->id)
            {
                auto a = arrows.find(id);
                if (a == arrows.end())
                {
                    Arrow* arrow = new (poolAlloc(&arrowAlloc)) Arrow();
                    arrow->id = id;
                    if (id == this->id)
                    {
                        arrow->main = true;
                        main = arrow;
                    }
                    arrow->name = std::string(payload);
                    payload += arrow->name.size() + 1;
                    arrow->type = static_cast<Arrows>(Messages::read<uint8_t>(&payload));
                    arrow->sync(&payload);
                    if (arrow->isMain())
                        continue;
                    arrows[id] = arrow;
                }
                else
                {
                    a->second->sync(&payload);
                }
            }
            else
            {
                if (main)
                    main->sync(&payload);
                else
                {
                    Arrow* arrow = new (poolAlloc(&arrowAlloc)) Arrow();
                    arrow->id = id;
                    arrow->main = true;
                    main = arrow;
                    arrow->name = std::string(payload);
                    payload += arrow->name.size() + 1;
                    arrow->type = static_cast<Arrows>(Messages::read<uint8_t>(&payload));
                    arrow->sync(&payload);
                }
            }
        }

    }

    void Goom::sync(const char** payload)
    {
        this->position = Messages::read<vec2>(payload);
        this->velocity = Messages::read<vec2>(payload);
        this->hp = Messages::read<float>(payload);
    }

    void Arrow::sync(const char** payload)
    {
        this->position = Messages::read<vec2>(payload);
        this->velocity = Messages::read<vec2>(payload);
        this->dashing = Messages::read<bool>(payload);
        this->level = Messages::read<uint8_t>(payload);
        this->hp = Messages::read<float>(payload);
        this->maxHp = Messages::read<float>(payload);
        this->shielding = Messages::read<bool>(payload);
        if (!this->main)
        {
            float rot = Messages::read<float>(payload);
            this->attackReady = this->shielding = Messages::read<bool>(payload);
            this->shieldReady = this->shielding = Messages::read<bool>(payload);
        }
    }

    void GameView::destroyGoom(uint32_t id)
    {
        auto g = gooms.find(id);
        if (id != this->id && g != gooms.end())
        {
            poolFree(&goomAlloc, (*g).second);
            gooms.erase(id);
        }
    }

    void GameView::destroyArrow(uint32_t id)
    {
        auto a = arrows.find(id);
        if (id != this->id && a != arrows.end())
        {
            poolFree(&arrowAlloc, (*a).second);
            arrows.erase(id);
        }
    }

    void GameView::reset()
    {
        poolFree(&arrowAlloc, main);
        main = nullptr;
        /*for (auto g : gooms)
          {
          poolFree(&goomAlloc, g.second);
          }
          gooms.clear();
          for (auto a : arrows)
          {
          poolFree(&arrowAlloc, a.second);
          }
          arrows.clear();*/
    }
}
