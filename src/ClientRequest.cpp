#include "ClientRequest.hpp"

using namespace std;

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
    {"connect", []() { return make_unique<Requests::Connect>(); }}
};

namespace Requests
{
    bool Connect::Parse(std::string_view str)
    {
        m_name = str;
        return true;
    }
}
