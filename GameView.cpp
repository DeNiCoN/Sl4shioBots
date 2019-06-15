/*
MIT License

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

void GameView::sync(const char* payload)
{
	//goo's
	for (int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	//new goo
		uint32_t id = Messages::read<uint32_t>(&payload);
		vec2 position = Messages::read<vec2>(&payload);
	}
	for (int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	//goo went out of screen
		uint32_t id = Messages::read<uint32_t>(&payload);
	}

	//gooms
	for (int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	//goom went out of screen

	}
	for (int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	//new goom

	}

	//arrows
	for (int i = 0; i < Messages::read<uint32_t>(&payload); i++)
	{	

	}

}

void Goom::sync(const char* payload)
{

}

void Arrow::sync(const char* payload)
{
}
