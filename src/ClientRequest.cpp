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
#include "ClientRequest.hpp"

using namespace std;

namespace Bots
{
    std::string_view readToken(std::string_view& str, char separator = ' ')
    {
        size_t pos = str.find(separator);
        std::string_view result = str.substr(0, pos);
        if (pos == str.npos || str.length() < pos + 1)
            str.remove_prefix(str.length());
        else
            str.remove_prefix(pos + 1);
        return result;
    }

    ClientRequestPtr ClientRequest::ParseRequest(std::string_view str)
    {
        auto it = s_requestsFac.find(readToken(str));
        if (it != s_requestsFac.end())
        {
            auto ptr = it->second();

            if (ptr->Parse(str))
                return ptr;
            return nullptr;
        }
        else
        {
            return nullptr;
        }
    }

    std::unordered_map<std::string_view,
                       std::function<ClientRequestPtr()>> ClientRequest::s_requestsFac =
    {
        {"test", []() { return make_unique<Requests::Test>(); }},
        {"connect", []() { return make_unique<Requests::Connect>(); }},
        {"stats", []() { return make_unique<Requests::Stats>(); }},
        {"state", []() { return make_unique<Requests::State>(); }},
        {"disconnect", []() { return make_unique<Requests::Disconnect>(); }}
    };

    namespace Requests
    {
        bool Connect::Parse(std::string_view str)
        {
            m_name = str;
            return true;
        }

        bool Disconnect::Parse(std::string_view str)
        {
            m_name = str;
            return true;
        }

        bool Stats::Parse(std::string_view str)
        {
            m_name = str;
            return true;
        }

        bool State::Parse(std::string_view str)
        {
            m_name = str;
            return true;
        }
    }
}
