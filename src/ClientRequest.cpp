#include "ClientRequest.hpp"

using namespace std;

ClientRequestPtr ClientRequest::ParseRequest(std::string_view str)
{
    size_t name = str.find(" ");
    auto it = s_requestsFac.find(str.substr(0, name));
    if (it != s_requestsFac.end())
    {
        auto ptr = it->second();
        str.remove_prefix(name);
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
    {"test", []() { return make_unique<TestRequest>(); }}
};
