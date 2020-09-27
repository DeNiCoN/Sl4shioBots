#include <cstdlib>
#include <cstring>
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: client <host> [port]\n";
            return 1;
        }

        asio::io_context io_context;

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        if (argc == 2)
            asio::connect(s, resolver.resolve(argv[1], std::to_string(34534)));
        else
            asio::connect(s, resolver.resolve(argv[1], argv[2]));

        while (true)
        {
            std::cout << "> ";
            std::string request;
            request.reserve(max_length);
            std::getline(std::cin, request);
            size_t request_length = request.length();
            if (request == "stop")
                break;
            asio::write(s, asio::buffer(request, request_length));

            char reply[max_length];
            size_t reply_length = s.read_some(asio::buffer(reply, max_length));
            std::cout << "< ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
