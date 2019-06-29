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
#include "GameView.h"
#include "World.h"
#include <algorithm>

const Goom* GameView::nearestGoom(vec2 position) const
{
	auto goom = std::min_element(gooms.begin(), gooms.end(), [position](std::pair<unsigned int, Goom*> left, std::pair<unsigned int, Goom*> right) -> bool
		{
			return vec2LengthSquared(vec2Sub(left.second->getPosition(), position)) < vec2LengthSquared(vec2Sub(right.second->getPosition(), position));
		});
	return (*goom).second;
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
		poolFree(&goomAlloc, gooms[id]);
		gooms.erase(id);
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

	//arrows
	for (unsigned int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	

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
}
